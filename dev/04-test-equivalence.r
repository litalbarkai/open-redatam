library(dplyr)
library(data.table)
library(ipumsr
library(janitor))

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

# Chile 2017 DIC ----

# https://redatam-ine.ine.cl/manuales/Manual-Usuario.pdf
# p08 = sex
# p09 = age

d_control_sex_chl <- d_control %>%
  filter(country == 152L, year == 2017) %>%
  group_by(sex) %>%
  summarise(n_ipums = sum(perwt)) %>%
  ungroup() %>%
  mutate(pct_ipums = n_ipums / sum(n_ipums) * 100)

d_control_age2_chl <- d_control %>%
  filter(country == 152L, year == 2017) %>%
  group_by(age2) %>%
  summarise(n_ipums = sum(perwt)) %>%
  ungroup() %>%
  mutate(pct_ipums = n_ipums / sum(n_ipums) * 100)

d_chile <- fread("downloads/redatam/CP2017CHL/open-redatam-dic-to-csv/PERSONA.csv.gz") %>%
  clean_names()

d_control_sex_chl <- d_control_sex_chl %>%
  bind_cols(
    d_chile %>%
      group_by(sex = p08) %>%
      count(name = "n_rdtm") %>%
      ungroup() %>%
      mutate(pct_rdtm = n_rdtm / sum(n_rdtm) * 100) %>%
      select(-sex)
  ) %>%
  mutate(
    n_diff = n_ipums - n_rdtm,
    pct_diff = pct_ipums - pct_rdtm
  )

d_control_age2_chl <- d_control_age2_chl %>%
  bind_cols(
    d_chile %>%
      mutate(
        p09 = case_when(
          p09 >= 0 & p09 <= 4   ~ 1,
          p09 >= 5 & p09 <= 9   ~ 2,
          p09 >= 10 & p09 <= 14 ~ 3,
          p09 >= 15 & p09 <= 19 ~ 4,
          p09 >= 20 & p09 <= 24 ~ 12,
          p09 >= 25 & p09 <= 29 ~ 13,
          p09 >= 30 & p09 <= 34 ~ 14,
          p09 >= 35 & p09 <= 39 ~ 15,
          p09 >= 40 & p09 <= 44 ~ 16,
          p09 >= 45 & p09 <= 49 ~ 17,
          p09 >= 50 & p09 <= 54 ~ 18,
          p09 >= 55 & p09 <= 59 ~ 19,
          p09 >= 60 & p09 <= 64 ~ 20,
          p09 >= 65 & p09 <= 69 ~ 21,
          p09 >= 70 & p09 <= 74 ~ 22,
          p09 >= 75 & p09 <= 79 ~ 23,
          p09 >= 80 & p09 <= 84 ~ 24,
          p09 >= 85             ~ 25
        )
      ) %>%
      group_by(age2 = p09) %>%
      count(name = "n_rdtm") %>%
      ungroup() %>%
      mutate(pct_rdtm = n_rdtm / sum(n_rdtm) * 100) %>%
      select(-age2)
  ) %>%
  mutate(
    n_diff = n_ipums - n_rdtm,
    pct_diff = pct_ipums - pct_rdtm
  )

rm(d_chile)

# Chile 2017 DICX ----

d_control_sex_chl <- d_control %>%
  filter(country == 152L, year == 2017) %>%
  group_by(sex) %>%
  summarise(n_ipums = sum(perwt)) %>%
  ungroup() %>%
  mutate(pct_ipums = n_ipums / sum(n_ipums) * 100)

d_control_age2_chl <- d_control %>%
  filter(country == 152L, year == 2017) %>%
  group_by(age2) %>%
  summarise(n_ipums = sum(perwt)) %>%
  ungroup() %>%
  mutate(pct_ipums = n_ipums / sum(n_ipums) * 100)

d_chile <- fread("downloads/redatam/CP2017CHL/open-redatam-dicx-to-csv/PERSONA.csv.gz") %>%
  clean_names()

d_control_sex_chl <- d_control_sex_chl %>%
  bind_cols(
    d_chile %>%
      group_by(sex = p08) %>%
      count(name = "n_rdtm") %>%
      ungroup() %>%
      mutate(pct_rdtm = n_rdtm / sum(n_rdtm) * 100) %>%
      select(-sex)
  ) %>%
  mutate(
    n_diff = n_ipums - n_rdtm,
    pct_diff = pct_ipums - pct_rdtm
  )

d_control_age2_chl <- d_control_age2_chl %>%
  bind_cols(
    d_chile %>%
      mutate(
        p09 = case_when(
          p09 >= 0 & p09 <= 4 ~ 1,
          p09 >= 5 & p09 <= 9 ~ 2,
          p09 >= 10 & p09 <= 14 ~ 3,
          p09 >= 15 & p09 <= 19 ~ 4,
          p09 >= 20 & p09 <= 24 ~ 12,
          p09 >= 25 & p09 <= 29 ~ 13,
          p09 >= 30 & p09 <= 34 ~ 14,
          p09 >= 35 & p09 <= 39 ~ 15,
          p09 >= 40 & p09 <= 44 ~ 16,
          p09 >= 45 & p09 <= 49 ~ 17,
          p09 >= 50 & p09 <= 54 ~ 18,
          p09 >= 55 & p09 <= 59 ~ 19,
          p09 >= 60 & p09 <= 64 ~ 20,
          p09 >= 65 & p09 <= 69 ~ 21,
          p09 >= 70 & p09 <= 74 ~ 22,
          p09 >= 75 & p09 <= 79 ~ 23,
          p09 >= 80 & p09 <= 84 ~ 24,
          p09 >= 85 ~ 25
        )
      ) %>%
      group_by(age2 = p09) %>%
      count(name = "n_rdtm") %>%
      ungroup() %>%
      mutate(pct_rdtm = n_rdtm / sum(n_rdtm) * 100) %>%
      select(-age2)
  ) %>%
  mutate(
    n_diff = n_ipums - n_rdtm,
    pct_diff = pct_ipums - pct_rdtm
  )

rm(d_chile)

# Bolivia 2001 DIC ----

# https://anda.ine.gob.bo/index.php/catalog/10/download/82
# sex = sexo
# age = edad

d_control_sex_bol <- d_control %>%
  filter(country == 68L, year == 2001) %>%
  group_by(sex) %>%
  summarise(n_ipums = sum(perwt)) %>%
  ungroup() %>%
  mutate(pct_ipums = n_ipums / sum(n_ipums) * 100)

d_control_age2_bol <- d_control %>%
  filter(country == 68L, year == 2001) %>%
  group_by(age2) %>%
  summarise(n_ipums = sum(perwt)) %>%
  ungroup() %>%
  mutate(pct_ipums = n_ipums / sum(n_ipums) * 100)

d_bolivia <- fread("downloads/redatam/CP2001BOL/open-redatam-dic-to-csv/PERSONA.csv.gz") %>%
  clean_names()

d_control_sex_bol <- d_control_sex_bol %>%
  bind_cols(
    d_bolivia %>%
      group_by(sexo) %>%
      count(name = "n_rdtm") %>%
      ungroup() %>%
      mutate(pct_rdtm = n_rdtm / sum(n_rdtm) * 100) %>%
      select(-sexo)
  ) %>%
  mutate(
    n_diff = n_ipums - n_rdtm,
    pct_diff = pct_ipums - pct_rdtm
  )

d_control_age2_bol <- d_control_age2_bol %>%
  bind_cols(
    d_bolivia %>%
      mutate(
        edad = case_when(
          edad >= 0 & edad <= 4 ~ 1,
          edad >= 5 & edad <= 9 ~ 2,
          edad >= 10 & edad <= 14 ~ 3,
          edad >= 15 & edad <= 19 ~ 4,
          edad >= 20 & edad <= 24 ~ 12,
          edad >= 25 & edad <= 29 ~ 13,
          edad >= 30 & edad <= 34 ~ 14,
          edad >= 35 & edad <= 39 ~ 15,
          edad >= 40 & edad <= 44 ~ 16,
          edad >= 45 & edad <= 49 ~ 17,
          edad >= 50 & edad <= 54 ~ 18,
          edad >= 55 & edad <= 59 ~ 19,
          edad >= 60 & edad <= 64 ~ 20,
          edad >= 65 & edad <= 69 ~ 21,
          edad >= 70 & edad <= 74 ~ 22,
          edad >= 75 & edad <= 79 ~ 23,
          edad >= 80 & edad <= 84 ~ 24,
          edad >= 85 ~ 25
        )
      ) %>%
      group_by(age2 = edad) %>%
      count(name = "n_rdtm") %>%
      ungroup() %>%
      mutate(pct_rdtm = n_rdtm / sum(n_rdtm) * 100) %>%
      select(-age2)
  )

rm(d_bolivia)

# Bolivia 2001 DICX ----

d_control_sex_bol <- d_control %>%
  filter(country == 68L, year == 2001) %>%
  group_by(sex) %>%
  summarise(n_ipums = sum(perwt)) %>%
  ungroup() %>%
  mutate(pct_ipums = n_ipums / sum(n_ipums) * 100)

d_control_age2_bol <- d_control %>%
  filter(country == 68L, year == 2001) %>%
  group_by(age2) %>%
  summarise(n_ipums = sum(perwt)) %>%
  ungroup() %>%
  mutate(pct_ipums = n_ipums / sum(n_ipums) * 100)

d_bolivia <- fread("downloads/redatam/CP2001BOL/open-redatam-dicx-to-csv/PERSONA.csv.gz") %>%
  clean_names()

d_control_sex_bol <- d_control_sex_bol %>%
  bind_cols(
    d_bolivia %>%
      group_by(sexo) %>%
      count(name = "n_rdtm") %>%
      ungroup() %>%
      mutate(pct_rdtm = n_rdtm / sum(n_rdtm) * 100) %>%
      select(-sexo)
  ) %>%
  mutate(
    n_diff = n_ipums - n_rdtm,
    pct_diff = pct_ipums - pct_rdtm
  )

d_control_age2_bol <- d_control_age2_bol %>%
  bind_cols(
    d_bolivia %>%
      mutate(
        edad = case_when(
          edad >= 0 & edad <= 4 ~ 1,
          edad >= 5 & edad <= 9 ~ 2,
          edad >= 10 & edad <= 14 ~ 3,
          edad >= 15 & edad <= 19 ~ 4,
          edad >= 20 & edad <= 24 ~ 12,
          edad >= 25 & edad <= 29 ~ 13,
          edad >= 30 & edad <= 34 ~ 14,
          edad >= 35 & edad <= 39 ~ 15,
          edad >= 40 & edad <= 44 ~ 16,
          edad >= 45 & edad <= 49 ~ 17,
          edad >= 50 & edad <= 54 ~ 18,
          edad >= 55 & edad <= 59 ~ 19,
          edad >= 60 & edad <= 64 ~ 20,
          edad >= 65 & edad <= 69 ~ 21,
          edad >= 70 & edad <= 74 ~ 22,
          edad >= 75 & edad <= 79 ~ 23,
          edad >= 80 & edad <= 84 ~ 24,
          edad >= 85 ~ 25
        )
      ) %>%
      group_by(age2 = edad) %>%
      count(name = "n_rdtm") %>%
      ungroup() %>%
      mutate(pct_rdtm = n_rdtm / sum(n_rdtm) * 100) %>%
      select(-age2)
  )

rm(d_bolivia)

# Bolivia 2012 DIC ----

# https://anda.ine.gob.bo/index.php/catalog/8/pdf-documentation
# sex = p24
# age = p25

d_control_sex_bol <- d_control %>%
  filter(country == 68L, year == 2012) %>%
  group_by(sex) %>%
  summarise(n_ipums = sum(perwt)) %>%
  ungroup() %>%
  mutate(pct_ipums = n_ipums / sum(n_ipums) * 100)

d_control_age2_bol <- d_control %>%
  filter(country == 68L, year == 2012) %>%
  group_by(age2) %>%
  summarise(n_ipums = sum(perwt)) %>%
  ungroup() %>%
  mutate(pct_ipums = n_ipums / sum(n_ipums) * 100)

d_bolivia <- fread("downloads/redatam/CP2012BOL/open-redatam-dic-to-csv/PERSONA.csv.gz") %>%
  clean_names()

d_control_sex_bol <- d_control_sex_bol %>%
  bind_cols(
    d_bolivia %>%
      group_by(p24) %>%
      count(name = "n_rdtm") %>%
      ungroup() %>%
      mutate(pct_rdtm = n_rdtm / sum(n_rdtm) * 100) %>%
      arrange(-p24) %>%
      select(-p24)
  ) %>%
  mutate(
    n_diff = n_ipums - n_rdtm,
    pct_diff = pct_ipums - pct_rdtm
  )

d_control_age2_bol <- d_control_age2_bol %>%
  bind_cols(
    d_bolivia %>%
      mutate(
        p25 = case_when(
          p25 >= 0 & p25 <= 4 ~ 1,
          p25 >= 5 & p25 <= 9 ~ 2,
          p25 >= 10 & p25 <= 14 ~ 3,
          p25 >= 15 & p25 <= 19 ~ 4,
          p25 >= 20 & p25 <= 24 ~ 12,
          p25 >= 25 & p25 <= 29 ~ 13,
          p25 >= 30 & p25 <= 34 ~ 14,
          p25 >= 35 & p25 <= 39 ~ 15,
          p25 >= 40 & p25 <= 44 ~ 16,
          p25 >= 45 & p25 <= 49 ~ 17,
          p25 >= 50 & p25 <= 54 ~ 18,
          p25 >= 55 & p25 <= 59 ~ 19,
          p25 >= 60 & p25 <= 64 ~ 20,
          p25 >= 65 & p25 <= 69 ~ 21,
          p25 >= 70 & p25 <= 74 ~ 22,
          p25 >= 75 & p25 <= 79 ~ 23,
          p25 >= 80 & p25 <= 84 ~ 24,
          p25 >= 85 ~ 25
        )
      ) %>%
      group_by(age2 = p25) %>%
      count(name = "n_rdtm") %>%
      ungroup() %>%
      mutate(pct_rdtm = n_rdtm / sum(n_rdtm) * 100) %>%
      select(-age2)
  )

rm(d_bolivia)

#### Bolivia 2012 DICX ----

d_control_sex_bol <- d_control %>%
  filter(country == 68L, year == 2012) %>%
  group_by(sex) %>%
  summarise(n_ipums = sum(perwt)) %>%
  ungroup() %>%
  mutate(pct_ipums = n_ipums / sum(n_ipums) * 100)

d_control_age2_bol <- d_control %>%
  filter(country == 68L, year == 2012) %>%
  group_by(age2) %>%
  summarise(n_ipums = sum(perwt)) %>%
  ungroup() %>%
  mutate(pct_ipums = n_ipums / sum(n_ipums) * 100)

d_bolivia <- fread("downloads/redatam/CP2012BOL/open-redatam-dicx-to-csv/PERSONA.csv.gz") %>%
  clean_names()

d_control_sex_bol <- d_control_sex_bol %>%
  bind_cols(
    d_bolivia %>%
      group_by(p24) %>%
      count(name = "n_rdtm") %>%
      ungroup() %>%
      mutate(pct_rdtm = n_rdtm / sum(n_rdtm) * 100) %>%
      arrange(-p24) %>%
      select(-p24)
  ) %>%
  mutate(
    n_diff = n_ipums - n_rdtm,
    pct_diff = pct_ipums - pct_rdtm
  )

d_control_age2_bol <- d_control_age2_bol %>%
  bind_cols(
    d_bolivia %>%
      mutate(
        p25 = case_when(
          p25 >= 0 & p25 <= 4 ~ 1,
          p25 >= 5 & p25 <= 9 ~ 2,
          p25 >= 10 & p25 <= 14 ~ 3,
          p25 >= 15 & p25 <= 19 ~ 4,
          p25 >= 20 & p25 <= 24 ~ 12,
          p25 >= 25 & p25 <= 29 ~ 13,
          p25 >= 30 & p25 <= 34 ~ 14,
          p25 >= 35 & p25 <= 39 ~ 15,
          p25 >= 40 & p25 <= 44 ~ 16,
          p25 >= 45 & p25 <= 49 ~ 17,
          p25 >= 50 & p25 <= 54 ~ 18,
          p25 >= 55 & p25 <= 59 ~ 19,
          p25 >= 60 & p25 <= 64 ~ 20,
          p25 >= 65 & p25 <= 69 ~ 21,
          p25 >= 70 & p25 <= 74 ~ 22,
          p25 >= 75 & p25 <= 79 ~ 23,
          p25 >= 80 & p25 <= 84 ~ 24,
          p25 >= 85 ~ 25
        )
      ) %>%
      group_by(age2 = p25) %>%
      count(name = "n_rdtm") %>%
      ungroup() %>%
      mutate(pct_rdtm = n_rdtm / sum(n_rdtm) * 100) %>%
      select(-age2)
  )

rm(d_bolivia)

#### Ecuador 2010 DIC ----

d_control_sex_ecu <- d_control %>%
  filter(country == 218L, year == 2010) %>%
  group_by(sex) %>%
  summarise(n_ipums = sum(perwt)) %>%
  ungroup() %>%
  mutate(pct_ipums = n_ipums / sum(n_ipums) * 100)

d_control_age2_ecu <- d_control %>%
  filter(country == 218L, year == 2010) %>%
  group_by(age2) %>%
  summarise(n_ipums = sum(perwt)) %>%
  ungroup() %>%
  mutate(pct_ipums = n_ipums / sum(n_ipums) * 100)

d_ecuador <- fread("downloads/redatam/CP2010ECU/open-redatam-dic-to-csv/PERSONA.csv.gz") %>%
  clean_names()

d_control_sex_ecu <- d_control_sex_ecu %>%
  bind_cols(
    d_ecuador %>%
      group_by(p01) %>%
      count(name = "n_rdtm") %>%
      ungroup() %>%
      mutate(pct_rdtm = n_rdtm / sum(n_rdtm) * 100) %>%
      select(-p01)
  ) %>%
  mutate(
    n_diff = n_ipums - n_rdtm,
    pct_diff = pct_ipums - pct_rdtm
  )

d_control_age2_ecu <- d_control_age2_ecu %>%
  bind_cols(
    d_ecuador %>%
      mutate(
        p03 = case_when(
          p03 >= 0 & p03 <= 4 ~ 1,
          p03 >= 5 & p03 <= 9 ~ 2,
          p03 >= 10 & p03 <= 14 ~ 3,
          p03 >= 15 & p03 <= 19 ~ 4,
          p03 >= 20 & p03 <= 24 ~ 12,
          p03 >= 25 & p03 <= 29 ~ 13,
          p03 >= 30 & p03 <= 34 ~ 14,
          p03 >= 35 & p03 <= 39 ~ 15,
          p03 >= 40 & p03 <= 44 ~ 16,
          p03 >= 45 & p03 <= 49 ~ 17,
          p03 >= 50 & p03 <= 54 ~ 18,
          p03 >= 55 & p03 <= 59 ~ 19,
          p03 >= 60 & p03 <= 64 ~ 20,
          p03 >= 65 & p03 <= 69 ~ 21,
          p03 >= 70 & p03 <= 74 ~ 22,
          p03 >= 75 & p03 <= 79 ~ 23,
          p03 >= 80 & p03 <= 84 ~ 24,
          p03 >= 85 ~ 25
        )
      ) %>%
      group_by(age2 = p03) %>%
      count(name = "n_rdtm") %>%
      ungroup() %>%
      mutate(pct_rdtm = n_rdtm / sum(n_rdtm) * 100) %>%
      select(-age2)
  )

rm(d_ecuador)

#### Ecuador 2010 DICX ----

d_control_sex_ecu <- d_control %>%
  filter(country == 218L, year == 2010) %>%
  group_by(sex) %>%
  summarise(n_ipums = sum(perwt)) %>%
  ungroup() %>%
  mutate(pct_ipums = n_ipums / sum(n_ipums) * 100)

d_control_age2_ecu <- d_control %>%
  filter(country == 218L, year == 2010) %>%
  group_by(age2) %>%
  summarise(n_ipums = sum(perwt)) %>%
  ungroup() %>%
  mutate(pct_ipums = n_ipums / sum(n_ipums) * 100)

d_ecuador <- fread("downloads/redatam/CP2010ECU/open-redatam-dicx-to-csv/PERSONA.csv.gz") %>%
  clean_names()

d_control_sex_ecu <- d_control_sex_ecu %>%
  bind_cols(
    d_ecuador %>%
      group_by(p01) %>%
      count(name = "n_rdtm") %>%
      ungroup() %>%
      mutate(pct_rdtm = n_rdtm / sum(n_rdtm) * 100) %>%
      select(-p01)
  ) %>%
  mutate(
    n_diff = n_ipums - n_rdtm,
    pct_diff = pct_ipums - pct_rdtm
  )

d_control_age2_ecu <- d_control_age2_ecu %>%
  bind_cols(
    d_ecuador %>%
      mutate(
        p03 = case_when(
          p03 >= 0 & p03 <= 4 ~ 1,
          p03 >= 5 & p03 <= 9 ~ 2,
          p03 >= 10 & p03 <= 14 ~ 3,
          p03 >= 15 & p03 <= 19 ~ 4,
          p03 >= 20 & p03 <= 24 ~ 12,
          p03 >= 25 & p03 <= 29 ~ 13,
          p03 >= 30 & p03 <= 34 ~ 14,
          p03 >= 35 & p03 <= 39 ~ 15,
          p03 >= 40 & p03 <= 44 ~ 16,
          p03 >= 45 & p03 <= 49 ~ 17,
          p03 >= 50 & p03 <= 54 ~ 18,
          p03 >= 55 & p03 <= 59 ~ 19,
          p03 >= 60 & p03 <= 64 ~ 20,
          p03 >= 65 & p03 <= 69 ~ 21,
          p03 >= 70 & p03 <= 74 ~ 22,
          p03 >= 75 & p03 <= 79 ~ 23,
          p03 >= 80 & p03 <= 84 ~ 24,
          p03 >= 85 ~ 25
        )
      ) %>%
      group_by(age2 = p03) %>%
      count(name = "n_rdtm") %>%
      ungroup() %>%
      mutate(pct_rdtm = n_rdtm / sum(n_rdtm) * 100) %>%
      select(-age2)
  )

rm(d_ecuador)

#### Uruguay 2011 DIC ----

d_control_sex_uru <- d_control %>%
  filter(country == 858L, year == 2011) %>%
  group_by(sex) %>%
  summarise(n_ipums = sum(perwt)) %>%
  ungroup() %>%
  mutate(pct_ipums = n_ipums / sum(n_ipums) * 100)

d_control_age2_uru <- d_control %>%
  filter(country == 858L, year == 2011) %>%
  group_by(age2) %>%
  summarise(n_ipums = sum(perwt)) %>%
  ungroup() %>%
  mutate(pct_ipums = n_ipums / sum(n_ipums) * 100)

d_uruguay <- fread("downloads/redatam/CP2011URY/open-redatam-dic-to-csv/PERSONA.csv.gz") %>%
  clean_names()

d_control_sex_uru <- d_control_sex_uru %>%
  bind_cols(
    d_uruguay %>%
      group_by(ph02) %>%
      count(name = "n_rdtm") %>%
      ungroup() %>%
      mutate(pct_rdtm = n_rdtm / sum(n_rdtm) * 100) %>%
      select(-ph02)
  ) %>%
  mutate(
    n_diff = n_ipums - n_rdtm,
    pct_diff = pct_ipums - pct_rdtm
  )

d_control_age2_uru <- d_control_age2_uru %>%
  bind_cols(
    d_uruguay %>%
      mutate(
        na01 = case_when(
          na01 >= 0 & na01 <= 4 ~ 1,
          na01 >= 5 & na01 <= 9 ~ 2,
          na01 >= 10 & na01 <= 14 ~ 3,
          na01 >= 15 & na01 <= 19 ~ 4,
          na01 >= 20 & na01 <= 24 ~ 12,
          na01 >= 25 & na01 <= 29 ~ 13,
          na01 >= 30 & na01 <= 34 ~ 14,
          na01 >= 35 & na01 <= 39 ~ 15,
          na01 >= 40 & na01 <= 44 ~ 16,
          na01 >= 45 & na01 <= 49 ~ 17,
          na01 >= 50 & na01 <= 54 ~ 18,
          na01 >= 55 & na01 <= 59 ~ 19,
          na01 >= 60 & na01 <= 64 ~ 20,
          na01 >= 65 & na01 <= 69 ~ 21,
          na01 >= 70 & na01 <= 74 ~ 22,
          na01 >= 75 & na01 <= 79 ~ 23,
          na01 >= 80 & na01 <= 84 ~ 24,
          na01 >= 85 ~ 25
        )
      ) %>%
      group_by(age2 = na01) %>%
      count(name = "n_rdtm") %>%
      ungroup() %>%
      mutate(pct_rdtm = n_rdtm / sum(n_rdtm) * 100) %>%
      select(-age2)
  ) %>%
  mutate(
    n_diff = n_ipums - n_rdtm,
    pct_diff = pct_ipums - pct_rdtm
  )

rm(d_uruguay)
