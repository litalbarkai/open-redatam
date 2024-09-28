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

load_all()

# Define the path to the .ptr file
absolute_path <- normalizePath("dev/BaseOrg16/CPV2017-1601.ptr")

# Define the paths to the .rbf files (as detected by your DIC/DICX parsing)
rbf_paths <- c(
  normalizePath("dev/BaseOrg16/CPV2017-1601.rbf"),
  normalizePath("dev/BaseOrg16/CPV2017-16_LKP.rbf"),
  normalizePath("dev/BaseOrg16/CPV2017-16_REGION_REDCODEN.rbf")
)

# Call the function with the .ptr file, the .rbf files, entity name, and parent entity name
foo <- print_rbf_paths_with_logging(absolute_path, rbf_paths, "REGION", "CENSO16R")
foo