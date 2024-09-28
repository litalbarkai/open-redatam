fix_encoding_ <- function(x) {
  if (is.list(x)) {
    return(lapply(x, fix_encoding_))
  } else if (is.character(x)) {
    return(iconv(x, sub = ""))
  } else {
    return(x)
  }
}

tidy_names_ <- function(x) {
  if (is.list(x)) {
    return(lapply(x, tidy_names_))
  } else if (is.character(x)) {
    return(janitor::make_clean_names(x))
  } else {
    return(x)
  }
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

remove_name_ <- function(dic) {
  dic <- lapply(dic, function(x) {
    if (is.list(x)) {
      # Recursively apply to sublists
      x <- remove_name_(x)
      # Remove elements with the name "name"
      if (!is.null(names(x))) {
        x <- x[names(x) != "name"]
      }
    }
    return(x)
  })
  return(dic)
}
