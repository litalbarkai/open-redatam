fix_encoding_ <- function(x) {
  if (is.list(x)) {
    return(lapply(x, fix_encoding_))
  } else if (is.character(x)) {
    return(iconv(x, sub = ""))
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
  } else if (is.character(x) && isTRUE(nchar(x) == 0L)) {
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

tidy_dic_dicx_ <- function(dictionary) {
  # 1. fix encoding (e.g., ""C\xf3digo Region" -> "Código Region")
  # 2. if an element is an empty list, remove it
  dic <- fix_encoding_(
    replace_empty_with_na_(
      if (grepl("\\.dic$|\\.DIC$", dictionary)) {
        read_dic_(dictionary)
      } else if (grepl("\\.dicx$|\\.DICX$||\\.dicX", dictionary)) {
        read_dicx_(dictionary)
      } else {
        stop("The file extension must be either .dic or .dicx")
      }
    )
  )

  # 3. remove list[["name"]] because the name is already in the list name
  dic <- remove_name_(dic)

  return(dic)
}

# subset_filename_elements_ <- function(dic) {
#   if (is.list(dic)) {
#     result <- list()
#     if (!is.null(dic$filename)) {
#       result$filename <- dic$filename
#     }
#     for (name in names(dic)) {
#       if (is.list(dic[[name]])) {
#         nested_result <- subset_filename_elements_(dic[[name]])
#         if (length(nested_result) > 0) {
#           result[[name]] <- nested_result
#         }
#       }
#     }
#     return(result)
#   }
#   return(NULL)
# }

subset_filename_elements_ <- function(dic) {
  # Initialize the result list
  result <- list()

  # Helper function to recursively collect filenames
  collect_filenames <- function(x) {
    filenames <- c()
    if (is.list(x)) {
      for (i in seq_along(x)) {
        if (is.list(x[[i]])) {
          filenames <- c(filenames, collect_filenames(x[[i]]))
        } else if (is.character(x[[i]]) && grepl("\\.ptr$|\\.rbf$", x[[i]])) {
          filenames <- c(filenames, x[[i]])
        }
      }
    }
    return(filenames)
  }

  # Iterate over the top-level elements
  for (name in names(dic)) {
    result[[name]] <- collect_filenames(dic[[name]])
  }

  return(result)
}
