library(dplyr)
library(data.table)
library(ipumsr)
library(janitor)

# IPUMS data ----

fout <- "downloads/ipums/ipumsi_00002.rds"

if (!file.exists(fout)) {
  d_control <- read_ipums_ddi("downloads/ipums/ipumsi_00002.xml")
  d_control <- read_ipums_micro(d_control)
  d_control <- clean_names(d_control)
  saveRDS(d_control, "downloads/ipums/ipumsi_00002.rds")
} else {
  d_control <- readRDS(fout)
}

countries <- d_control %>%
  distinct(country)

# Function ----

generate_tables <- function(d_control, file_name, country_code, census_year, var_sex, var_age) {
  # Process d_control for sex
  d_control_sex <- d_control %>%
    filter(country == country_code, year == census_year) %>%
    group_by(sex) %>%
    summarise(n_ipums = sum(perwt)) %>%
    ungroup() %>%
    mutate(pct_ipums = n_ipums / sum(n_ipums) * 100)
  
  # Process d_control for age
  d_control_age <- d_control %>%
    filter(country == country_code, year == census_year) %>%
    group_by(age2) %>%
    summarise(n_ipums = sum(perwt)) %>%
    ungroup() %>%
    mutate(pct_ipums = n_ipums / sum(n_ipums) * 100)
  
  # Read and clean the external data
  d_external <- fread(file_name) %>%
    clean_names()
  
  d_sex <- d_external %>%
    group_by(sex = !!sym(var_sex)) %>%
    count(name = "n_rdtm") %>%
    ungroup() %>%
    mutate(pct_rdtm = n_rdtm / sum(n_rdtm) * 100)

  if (file_name == "downloads/redatam/CP2012BOL/open-redatam-dicx-to-csv/PERSONA.csv.gz") {
    d_sex$sex <- c(2L,1L)
  }

  # Bind columns and calculate differences for sex
  d_control_sex <- d_control_sex %>%
    left_join(d_sex) %>%
    mutate(
      n_diff = n_ipums - n_rdtm,
      pct_diff = pct_ipums - pct_rdtm
    )
  
  # Bind columns and calculate differences for age
  d_control_age <- d_control_age %>%
    left_join(
      d_external %>%
        mutate(
          age2 = case_when(
            !!sym(var_age) >= 0 & !!sym(var_age) <= 4   ~ 1,
            !!sym(var_age) >= 5 & !!sym(var_age) <= 9   ~ 2,
            !!sym(var_age) >= 10 & !!sym(var_age) <= 14 ~ 3,
            !!sym(var_age) >= 15 & !!sym(var_age) <= 19 ~ 4,
            !!sym(var_age) >= 20 & !!sym(var_age) <= 24 ~ 12,
            !!sym(var_age) >= 25 & !!sym(var_age) <= 29 ~ 13,
            !!sym(var_age) >= 30 & !!sym(var_age) <= 34 ~ 14,
            !!sym(var_age) >= 35 & !!sym(var_age) <= 39 ~ 15,
            !!sym(var_age) >= 40 & !!sym(var_age) <= 44 ~ 16,
            !!sym(var_age) >= 45 & !!sym(var_age) <= 49 ~ 17,
            !!sym(var_age) >= 50 & !!sym(var_age) <= 54 ~ 18,
            !!sym(var_age) >= 55 & !!sym(var_age) <= 59 ~ 19,
            !!sym(var_age) >= 60 & !!sym(var_age) <= 64 ~ 20,
            !!sym(var_age) >= 65 & !!sym(var_age) <= 69 ~ 21,
            !!sym(var_age) >= 70 & !!sym(var_age) <= 74 ~ 22,
            !!sym(var_age) >= 75 & !!sym(var_age) <= 79 ~ 23,
            !!sym(var_age) >= 80 & !!sym(var_age) <= 84 ~ 24,
            !!sym(var_age) >= 85             ~ 25
          )
        ) %>%
        group_by(age2) %>%
        count(name = "n_rdtm") %>%
        ungroup() %>%
        mutate(pct_rdtm = n_rdtm / sum(n_rdtm) * 100)
    ) %>%
    mutate(
      n_diff = n_ipums - n_rdtm,
      pct_diff = pct_ipums - pct_rdtm
    )
  
  # Return the tables
  return(list(d_control_sex = d_control_sex, d_control_age = d_control_age))
}

generate_tables_r <- function(d_control, country_code, census_year, var_sex, var_age) {
  # just Chile, it is identical to the other validation function
  file_name <- "downloads/CP2017CHL/BaseOrg16/CPV2017-16.dic"
  # Process d_control for sex
  d_control_sex <- d_control %>%
    filter(country == country_code, year == census_year) %>%
    group_by(sex) %>%
    summarise(n_ipums = sum(perwt)) %>%
    ungroup() %>%
    mutate(pct_ipums = n_ipums / sum(n_ipums) * 100)

  # Process d_control for age
  d_control_age <- d_control %>%
    filter(country == country_code, year == census_year) %>%
    group_by(age2) %>%
    summarise(n_ipums = sum(perwt)) %>%
    ungroup() %>%
    mutate(pct_ipums = n_ipums / sum(n_ipums) * 100)

  # Read and clean the external data
  d_external <- redatam::read_redatam(file_name)

  names(d_external)

  d_sex <- d_external$persona %>%
    group_by(sex = !!sym(var_sex)) %>%
    count(name = "n_rdtm") %>%
    ungroup() %>%
    mutate(pct_rdtm = n_rdtm / sum(n_rdtm) * 100)

  # Bind columns and calculate differences for sex
  d_control_sex <- d_control_sex %>%
    left_join(d_sex) %>%
    mutate(
      n_diff = n_ipums - n_rdtm,
      pct_diff = pct_ipums - pct_rdtm
    )

  # Bind columns and calculate differences for age
  d_control_age <- d_control_age %>%
    left_join(
      d_external$persona %>%
        mutate(
          age2 = case_when(
            !!sym(var_age) >= 0 & !!sym(var_age) <= 4 ~ 1,
            !!sym(var_age) >= 5 & !!sym(var_age) <= 9 ~ 2,
            !!sym(var_age) >= 10 & !!sym(var_age) <= 14 ~ 3,
            !!sym(var_age) >= 15 & !!sym(var_age) <= 19 ~ 4,
            !!sym(var_age) >= 20 & !!sym(var_age) <= 24 ~ 12,
            !!sym(var_age) >= 25 & !!sym(var_age) <= 29 ~ 13,
            !!sym(var_age) >= 30 & !!sym(var_age) <= 34 ~ 14,
            !!sym(var_age) >= 35 & !!sym(var_age) <= 39 ~ 15,
            !!sym(var_age) >= 40 & !!sym(var_age) <= 44 ~ 16,
            !!sym(var_age) >= 45 & !!sym(var_age) <= 49 ~ 17,
            !!sym(var_age) >= 50 & !!sym(var_age) <= 54 ~ 18,
            !!sym(var_age) >= 55 & !!sym(var_age) <= 59 ~ 19,
            !!sym(var_age) >= 60 & !!sym(var_age) <= 64 ~ 20,
            !!sym(var_age) >= 65 & !!sym(var_age) <= 69 ~ 21,
            !!sym(var_age) >= 70 & !!sym(var_age) <= 74 ~ 22,
            !!sym(var_age) >= 75 & !!sym(var_age) <= 79 ~ 23,
            !!sym(var_age) >= 80 & !!sym(var_age) <= 84 ~ 24,
            !!sym(var_age) >= 85 ~ 25
          )
        ) %>%
        group_by(age2) %>%
        count(name = "n_rdtm") %>%
        ungroup() %>%
        mutate(pct_rdtm = n_rdtm / sum(n_rdtm) * 100)
    ) %>%
    mutate(
      n_diff = n_ipums - n_rdtm,
      pct_diff = pct_ipums - pct_rdtm
    )

  # Return the tables
  return(list(d_control_sex = d_control_sex, d_control_age = d_control_age))
}

# Chile 2017 ----

# https://redatam-ine.ine.cl/manuales/Manual-Usuario.pdf
# p08 = sex
# p09 = age

result <- generate_tables(d_control,
  # "downloads/redatam/CP2017CHL/open-redatam-dic-to-csv/PERSONA.csv.gz",
  "downloads/redatam/CP2017CHL/open-redatam-dicx-to-csv/PERSONA.csv.gz",
  152L,
  2017L,
  "p08",
  "p09"
)

result <- generate_tables_r(
  d_control,
  152L,
  2017L,
  "p08",
  "p09"
)

# Bolivia 2001 ----

result <- generate_tables(
  d_control,
  # "downloads/redatam/CP2001BOL/open-redatam-dic-to-csv/PERSONA.csv.gz",
  "downloads/redatam/CP2001BOL/open-redatam-dicx-to-csv/PERSONA.csv.gz",
  68L,
  2001L,
  "sexo",
  "edad"
)

# Bolivia 2012 ----

result <- generate_tables(
  d_control,
  # "downloads/redatam/CP2012BOL/open-redatam-dic-to-csv/PERSONA.csv.gz",
  "downloads/redatam/CP2012BOL/open-redatam-dicx-to-csv/PERSONA.csv.gz",
  68L,
  2012L,
  "p24",
  "p25"
)

# Dominican Republic 2002 ----

result <- generate_tables(
  d_control,
  # "downloads/redatam/CP2002DOM/open-redatam-dic-to-csv/PERSONA.csv.gz",
  "downloads/redatam/CP2002DOM/open-redatam-dicx-to-csv/PERSONA.csv.gz",
  214L,
  2002L,
  "p28",
  "p29d"
)

# Ecuador 2010 DIC ----

result <- generate_tables(
  d_control,
  # "downloads/redatam/CP2010ECU/open-redatam-dic-to-csv/PERSONA.csv.gz",
  "downloads/redatam/CP2010ECU/open-redatam-dicx-to-csv/PERSONA.csv.gz",
  218L,
  2010L,
  "p01",
  "p03"
)

# El Salvador 2007 ----

result <- generate_tables(
  d_control,
  "downloads/redatam/CP2007SLV/open-redatam-dic-to-csv/PERSONA.csv.gz",
  # "downloads/redatam/CP2007SLV/open-redatam-dicx-to-csv/PERSONA.csv.gz",
  222L,
  2007L,
  "p02",
  "p03a"
)

# Peru 2007 ----

result <- generate_tables(
  d_control,
  # "downloads/redatam/CP2007PER/open-redatam-dic-to-csv/Poblacio.csv.gz",
  "downloads/redatam/CP2007PER/open-redatam-dicx-to-csv/Poblacio.csv.gz",
  604L,
  2007L,
  "p02sexo",
  "p03aanio"
)

# Uruguay 2011 DIC ----

result <- generate_tables(
  d_control,
  # "downloads/redatam/CP2011URY/open-redatam-dic-to-csv/PERSONA.csv.gz",
  "downloads/redatam/CP2011URY/open-redatam-dicx-to-csv/PERSONA.csv.gz",
  858L,
  2011L,
  "ph02",
  "na01"
)
