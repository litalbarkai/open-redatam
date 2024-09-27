if (!require("cpp11")) {
  install.packages("cpp11")
}

if (!require("devtools")) {
  install.packages("devtools")
}

if (getwd() != "/home/pacha/github/redatam-converter/rpkg") {
  setwd("rpkg")
}

cpp11::cpp_register()
devtools::document()

devtools::load_all()

chl2 <- tidy_dic_dicx_("dev/BaseOrg16/CPV2017-16.dicx")

chl2$REGION

chl2files <- subset_filename_elements_(chl2)

chl2files$REGION

# chl2files$REGION[1]
# dev/BaseOrg16/CPV2017-1601.ptr

result <- parse_ptr_("dev/BaseOrg16/CPV2017-1601.ptr")
