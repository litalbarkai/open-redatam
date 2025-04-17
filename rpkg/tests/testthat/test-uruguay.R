# the Galapagos census was downloaded from
# https://redatam.org/cdr/descargas/censos/poblacion/CP2011URY.zip
# I agregated the data to test the code

# read DICX
# when converting the original DIC to DICX in REDATAM 7:
# 1. it doesn't include all the labels
# 2. it adds MISSING and NOTAPPLICABLE to the labels
# 3. it creates problems with lower and upper case (e.g., CG150001.ptr and
#    cg150001.ptr)

test_that("reading works", {
  zip <- system.file("extdata", "uru2011mini.zip", package = "redatam")

  dout <- paste(tempdir(), "uru2011mini", sep = "/")

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
  
  dicx <- list.files(dout, pattern = "\\.dicx$", full.names = TRUE,
    recursive = TRUE)

  # read DIC

  res <- read_redatam(dic)

  expect_type(res, "list")
  expect_equal(length(res), 3L)

  d <- res$sexo
  expect_true(is.data.frame(d))
  expect_true(is.factor(d$sexo))
  expect_equal(dim(d), c(38L,4L))

  daux <- res$sexo_labels_cuenta
  expect_true(is.factor(daux$cuenta_description))
  expect_equal(dim(daux), c(2L,2L))

  # read DICX
  
  res2 <- read_redatam(dicx)

  expect_type(res2, "list")
  expect_equal(length(res2), 2L)

  d2 <- res2$sexo
  expect_true(is.data.frame(d2))
  expect_equal(dim(d2), c(38L,4L))

  # copy .dic to .DIC
  file.copy(dic, gsub("\\.dic$", ".DIC", dic), overwrite = TRUE)

  res3 <- read_redatam(gsub("\\.dic$", ".DIC", dic))
  expect_type(res2, "list")
})
