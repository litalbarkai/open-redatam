tidy_dic_dicx_ <- function(dictionary) {
  # 1. fix encoding (e.g., ""C\xf3digo Region" -> "Código Region")
  # 2. if an element is an empty list, remove it
  dic <- replace_empty_with_na_(
    fix_encoding_(
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
