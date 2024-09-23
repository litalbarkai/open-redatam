fix_encoding_ <- function(x) {
  if (is.list(x)) {
    return(lapply(x, fix_encoding_))
  } else if (is.character(x)) {
    return(iconv(x, sub = ""))
  } else {
    return(x)
  }
}

discard_empty_ <- function(x) {
  if (is.list(x)) {
    x <- lapply(x, discard_empty)
    x <- x[!sapply(x, function(y) is.null(y) || (is.list(y) && length(y) == 0)
      || (is.character(y) && length(y) == 0))]
  }
  return(x)
}

tidy_dic_dicx_ <- function(dictionary) {
  # 1. fix encoding (e.g., ""C\xf3digo Region" -> "Código Region")
  # 2. if an element is an empty list, remove it
  dic <- fix_encoding_(
    discard_empty_(
      if (grepl("\\.dic$|\\.DIC$", dictionary)) {
        read_dic_(dictionary)
      } else if (grepl("\\.dicx$|\\.DICX$||\\.dicX", dictionary)) {
        read_dicx_(dictionary)
      } else {
        stop("The file extension must be either .dic or .dicx")
      }
    )
  )

  return(dic)
}
