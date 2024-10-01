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

# dic <- "dev/BaseOrg16/CPV2017-16.dic"
# dic <- "dev/BaseOrg16/CPV2017-16.dicx"
dic <- "dev/CP2011URY/BaseRPub/CPV2011_uruguay_publica.dicX"
dic <- normalizePath(dic)

dout <- "dev/dicx-from-r"
try(dir.create(dout))
dout <- normalizePath(dout)

res <- read_redatam(dic)

res
