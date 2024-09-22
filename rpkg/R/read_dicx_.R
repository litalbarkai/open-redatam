get_var_type <- function(type) {
  switch(type,
    "CHR" = "STRING",
    "DBL" = "REAL",
    "INT" = "INT16",
    "LNG" = "INTEGER",
    "BIN" = "INTEGER",
    "PCK" = "INTEGER",
    "NA" = "",
    ""
  )
}

read_dicx_ <- function(filepath) {
  dicx <- read_xml(filepath)

  result <- list()

  root <- xml_find_all(dicx, ".//root")

  root_name <- xml_text(xml_find_first(root, ".//name"))
  root_pointer <- xml_find_first(root, ".//filename")

  result[[root_name]] <- list(
    name = root_name,
    pointer = xml_text(root_pointer)
  )

  variable <- xml_find_first(dicx, ".//variable")

  variable_name <- xml_text(xml_find_first(variable, ".//name"))
  variable_label <- xml_text(xml_find_first(variable, ".//label"))
  variable_type <- xml_text(xml_find_first(variable, ".//varType"))
  variable_field_size <- xml_text(xml_find_first(variable, ".//fieldSize"))

  variable_filename <- xml_text(xml_find_first(variable, ".//filename"))
  variable_filename <- gsub("\\.\\\\", paste0(dirname(filepath), "/"), variable_filename)
  variable_filename <- gsub("//", "/", variable_filename)

  result[[root_name]][[variable_name]] <- list(
    name = variable_name,
    label = variable_label,
    type = variable_type,
    fieldSize = variable_field_size,
    filename = variable_filename
  )

  entities <- xml_find_all(dicx, ".//entity")

  for (entity in entities) {
    entity_name <- xml_text(xml_find_first(entity, ".//name"))
    entity_label <- xml_text(xml_find_first(entity, ".//label"))

    entity_filename <- xml_text(xml_find_first(entity, ".//filename"))
    entity_filename <- gsub("\\.\\\\", paste0(dirname(filepath), "/"), entity_filename)
    entity_filename <- gsub("//", "/", entity_filename)

    variables <- xml_find_all(entity, ".//variable")
    variable_list <- lapply(variables, function(var) {
      var_name <- xml_text(xml_find_first(var, ".//name"))
      var_label <- xml_text(xml_find_first(var, ".//label"))
      
      var_type <- xml_find_first(var, ".//varDicChoice")
      var_type <- xml_text(xml_find_first(var_type, ".//datasetType"))
      var_type <- get_var_type(var_type)
      
      field_size <- xml_text(xml_find_first(var, ".//fieldSize"))

      var_filename <- xml_text(xml_find_first(var, ".//filename"))
      var_filename <- gsub("\\.\\\\", paste0(dirname(filepath), "/"), var_filename)
      var_filename <- gsub("//", "/", var_filename)

      value_labels <- xml_find_all(var, ".//valueLabelElement")
      value_label_list <- lapply(value_labels, function(value_label) {
        value <- xml_text(xml_find_first(value_label, ".//value"))
        label <- xml_text(xml_find_first(value_label, ".//label"))
        list(value = value, label = label)
      })

      res <- list(
        name = var_name,
        label = var_label,
        type = var_type,
        fieldsize = field_size,
        filename = var_filename,
        valuelabels = value_label_list
      )

      # names(res) <- res$name

      return(res)
    })

    names(variable_list) <- sapply(variable_list, function(x) x[[1]])

    result[[root_name]][[variable_name]][[entity_name]] <- list(
      name = entity_name,
      label = entity_label,
      filename = entity_filename,
      variables = variable_list
    )
  }

  return(result)
}
