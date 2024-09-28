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

# TEST DIC ----

dictionary <- "dev/BaseOrg16/CPV2017-16.dic"

chl <- tidy_dic_dicx_(dictionary)

chl$REGION

names(chl$REGION$variables)

x1 <- read_rbf_(chl$REGION$variables$IDREGION)
x2 <- read_rbf_(chl$REGION$variables$NREGION)
x3 <- read_rbf_(chl$REGION$variables$REDCODEN)

tibble::tibble(x1, x2, x3)

read_entity_(chl$REGION$variables)

# TEST DICX ----

chl2 <- tidy_dic_dicx_("dev/BaseOrg16/CPV2017-16.dicx")

chl2$REGION

# names(chl2$REGION$variables)

# x1 <- read_rbf_(chl2$REGION$variables$IDREGION)
# x2 <- read_rbf_(chl2$REGION$variables$NREGION)
# x3 <- read_rbf_(chl2$REGION$variables$REDCODEN)

tibble::tibble(x1, x2, x3)

rm(x1,x2,x3)

entities <- tidy_entities_(chl2)
