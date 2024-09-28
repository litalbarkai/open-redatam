# tibble::as_tibble(as.data.frame(read_entity_(chl$REGION)))

tidy_entities_ <- function(dictionary) {
  entities <- names(dictionary)

  out <- list()

  for (entity in entities) {
    message(paste0("Processing entity: ", entity, " (",
      length(dictionary[[entity]][["variables"]]), "variables)"))

    out[[entity]] <- read_entity_(dictionary[[entity]])

    # fix the encoding for each column
    out[[entity]] <- lapply(out[[entity]], function(x) {
      if (is.character(x)) {
        # I use stringi to avoid problems with encoding on Windows
        # return(iconv(x, from = "latin1", sub = ""))
        return(iconv(x, from = stringi::stri_enc_detect(x)[[1]]$Encoding[1], to = "UTF-8", sub = ""))
      } else {
        return(x)
      }
    })

    # replace multiple spaces with a single space +
    # remove leading and trailing spaces
    out[[entity]] <- lapply(out[[entity]], function(x) {
      if (is.character(x)) {
        return(trimws(gsub("\\s+", " ", x)))
      } else {
        return(x)
      }
    })

    out[[entity]] <- as.data.frame(out[[entity]])

    class(out[[entity]]) <- c("tbl_df", "tbl", "data.frame")
  }

  return(out)
}
