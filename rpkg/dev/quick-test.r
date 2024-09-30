if (!require("cpp11")) {
  install.packages("cpp11")
}

if (!require("devtools")) {
  install.packages("devtools")
}

if (getwd() != "/home/pacha/github/redatam-converter/rpkg") {
  setwd("rpkg")
}

devtools::clean_dll()
cpp11::cpp_register()
devtools::document()

devtools::load_all()

# chl2 <- tidy_dic_dicx_("dev/BaseOrg16/CPV2017-16.dicx")
# saveRDS(chl2, "dev/chl2.rds")

chl2 <- readRDS("dev/chl2.rds")

chl2$region

names(chl2$region$variables)

# out <- tidy_entities_(chl2)

dictionary <- chl2

entities <- names(dictionary)

entities

out <- list()

entity <- "region"
entity <- "provinci"
entity <- "comuna"
entity <- "distrito"
entity <- "area"

out[[entity]] <- read_entity_(dictionary[[entity]])

out
