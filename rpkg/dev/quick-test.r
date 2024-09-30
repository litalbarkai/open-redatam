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

chl$region

names(chl$region$variables)

# moved to an internal function
# x1 <- read_rbf_(chl$REGION$variables$IDREGION)
# x2 <- read_rbf_(chl$REGION$variables$NREGION)
# x3 <- read_rbf_(chl$REGION$variables$REDCODEN)
# tibble::tibble(x1, x2, x3)

read_entity_(chl$REGION$variables)

# TEST DICX ----

# chl2 <- tidy_dic_dicx_("dev/BaseOrg16/CPV2017-16.dicx")
# saveRDS(chl2, "dev/chl2.rds")

chl2 <- readRDS("dev/chl2.rds")

# chl2$REGION

chl2$region

names(chl2$region$variables)

# moved to an internal functionchl2 <- readRDS("dev/chl2.rds")

# x1 <- read_rbf_(chl2$REGION$variables$IDREGION)
# x2 <- read_rbf_(chl2$REGION$variables$NREGION)
# x3 <- read_rbf_(chl2$REGION$variables$REDCODEN)
# tibble::tibble(x1, x2, x3)
# rm(x1,x2,x3)

out <- tidy_entities_(chl2)

out <- read_ptr_file_("dev/BaseOrg16/CPV2017-1601.ptr")

entity_list <- read_entity_file("dev/BaseOrg16/CPV2017-1601.ptr")

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

out[[entity]] <- lapply(out[[entity]], function(x) {
  if (is.character(x)) {
    # I use stringi to avoid problems with encoding on Windows
    # return(iconv(x, from = "latin1", sub = ""))
    return(iconv(x, from = stringi::stri_enc_detect(x)[[1]]$Encoding[1], to = "UTF-8", sub = ""))
  } else {
    return(x)
  }
})

 out[[entity]] <- lapply(out[[entity]], function(x) {
   if (is.character(x)) {
     return(trimws(gsub("\\s+", " ", x)))
   } else {
     return(x)
   }
 })

out[[entity]] <- as.data.frame(out[[entity]])

class(out[[entity]]) <- c("tbl_df", "tbl", "data.frame")

out[[entity]]