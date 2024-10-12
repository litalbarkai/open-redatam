# the Galapagos census was downloaded from
# https://www.ecuadorencifras.gob.ec/documentos/datos/Poblacion_y_Demografia/Censo_poblacion_y_vivienda_/Galapagos_2015/bdd_CPV_Galapagos_AL15_AT_REDATAM.zip
# I removed some levels to reduce the package size

# read DICX
# when converting the original DIC to DICX in REDATAM 7:
# 1. it doesn't include all the labels
# 2. it adds MISSING and NOTAPPLICABLE to the labels
# 3. it creates problems with lower and upper case (e.g., CG150001.ptr and
#    cg150001.ptr)

test_that("reading works", {
  zip <- system.file("extdata", "galapagos.zip", package = "redatam")

  dout <- paste(tempdir(), "galapagos", sep = "/")

  if (file.exists(dout)) {
    unlink(dout, recursive = T)
  }

  # unzip the file
  if (!file.exists(dout)) {
    unzip(zip, exdir = dout)
  }

  # find the dictionary
  dic <- list.files(dout, pattern = "\\.dic$", full.names = TRUE,
    recursive = TRUE)
  
  dicx <- list.files(dout, pattern = "\\.dicX$", full.names = TRUE,
    recursive = TRUE)

  # read DIC

  res <- read_redatam(dic)

  expect_type(res, "list")
  expect_equal(length(res), 7L)

  d <- res$canton
  expect_true(is.data.frame(d))
  expect_true(is.factor(d$nomcant))
  expect_equal(dim(d), c(3L,4L))

  daux <- res$canton_labels_nomcant
  expect_true(is.factor(daux$nomcant))
  expect_true(is.factor(daux$nomcant_description))
  expect_equal(dim(daux), c(1L,2L))

  # read DICX
  
  res2 <- read_redatam(dicx)

  expect_type(res2, "list")
  expect_equal(length(res2), 7L)

  d2 <- res2$canton
  expect_true(is.data.frame(d2))
  expect_true(is.factor(d2$nomcant))
  expect_equal(dim(d2), c(3L,4L))

  daux2 <- res2$canton_labels_nomcant
  expect_true(is.factor(daux2$nomcant))
  expect_true(is.factor(daux2$nomcant_description))
  expect_equal(dim(daux2), c(3L,2L))
})
