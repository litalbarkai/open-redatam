# fix_encoding_ <- function(x) {
#   if (is.list(x)) {
#     return(lapply(x, fix_encoding_))
#   } else if (is.character(x)) {
#     # this adds too much overhead
#     # return(iconv(x, from = stringi::stri_enc_detect(x)[[1]]$Encoding[1], to = "UTF-8", sub = ""))
#     return(iconv(x, to = "UTF-8", sub = ""))
#   } else {
#     return(x)
#   }
# }

detect_encoding_ <- function(strings) {
  valid_strings <- paste(strings[!is.na(strings) & strings != ""], collapse = " ")
  if (length(valid_strings) > 0) {
    detected_encoding <- stringi::stri_enc_detect(valid_strings)
    if (length(detected_encoding) > 0) {
      return(detected_encoding[[1]]$Encoding[1])
    }
  }
  return("UTF-8")
}

fix_encoding_recursive_ <- function(x, encoding) {
  if (is.list(x)) {
    return(lapply(x, fix_encoding_recursive_, encoding))
  } else if (is.character(x)) {
    return(iconv(x, from = encoding, to = "UTF-8", sub = ""))
  } else {
    return(x)
  }
}

fix_encoding_ <- function(x) {
  # extract the unique values of the "chr" vectors in the lists
  strings <- unique(unlist(lapply(x, function(y) y[sapply(y, is.character)])))

  # detect the encoding of the strings
  encoding <- detect_encoding_(strings)
  

  return(fix_encoding_recursive_(x, encoding))
}

replace_empty_with_na_ <- function(x) {
  if (is.list(x)) {
    x <- lapply(x, replace_empty_with_na_) # Recursively apply to sublists

    x <- lapply(x, function(y) {
      if (is.list(y) && length(y) == 0L) {
        return(NA)
      } else {
        return(y)
      }
    })
  } else if (is.character(x) && isFALSE(nzchar(x))) {
    return(NA)
  } else if (is.null(x)) {
    return(NA)
  }

  return(x)
}

tidy_names_ <- function(x) {
  if (is.list(x)) {
    element_names <- names(x)

    # Check for NULL or empty names and replace them with temporary placeholders
    if (is.null(element_names)) {
      element_names <- rep("", length(x))
    }

    # Apply janitor to the non-empty names
    cleaned_names <- ifelse(element_names == "", element_names, janitor::make_clean_names(element_names))

    names(x) <- cleaned_names

    return(lapply(x, tidy_names_))
  } else {
    return(x)
  }
}

trim_and_clean_ <- function(x) {
  if (is.list(x)) {
    return(lapply(x, trim_and_clean_))
  } else if (is.character(x)) {
    return(trimws(gsub("\\s+", " ", x)))
  } else {
    return(x)
  }
}

list_to_tibble_ <- function(x) {
  for (y in names(x)) {
    if (is.list(x[[y]])) {
      x[[y]] <- as.data.frame(x[[y]])
      class(x[[y]]) <- c("tbl_df", "tbl", "data.frame")
    }
  }
  return(x)
}
