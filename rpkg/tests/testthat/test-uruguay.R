test_that("reading works", {
  zip <- system.file("extdata", "uruguay.zip", package = "redatam")

  dout <- paste(tempdir(), "uruguay", sep = "/")

  # unlink(dout, recursive = T)

  # unzip the file
  if (!file.exists(dout)) {
    unzip(zip, exdir = dout)
  }

  # find the dictionary
  dicx <- list.files(dout, pattern = "\\.dicx$", full.names = TRUE,
    recursive = TRUE)

  # read the data
  res <- suppressWarnings(read_redatam(dicx))

  expect_type(res, "list")
  expect_equal(length(res), 5L)

  d <- res$depto
  expect_true(is.data.frame(d))
  expect_true(is.factor(d$ndepto))
  expect_equal(dim(d), c(19L,6L))

  daux <- res$depto_labels_depto
  expect_true(is.factor(daux$depto_description))
  expect_equal(dim(daux), c(21L,2L))
})
