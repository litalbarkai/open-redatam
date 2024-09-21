read_dicx_ <- function(filepath) {
  # Parse the XML file
  dicx <- read_xml(filepath)

  # Initialize an empty list to store the result
  result <- list()

  # Loop through each entity
  entities <- xml_find_all(dicx, ".//entity")

  for (entity in entities) {
    entity_name <- xml_text(xml_find_first(entity, ".//name"))
    entity_label <- xml_text(xml_find_first(entity, ".//label"))
    entity_filename <- xml_text(xml_find_first(entity, ".//filename"))
    entity_tag <- xml_text(xml_find_first(entity, ".//tag"))

    # Extract variable information
    variables <- xml_find_all(entity, ".//variable")
    variable_list <- lapply(variables, function(var) {
      var_name <- xml_text(xml_find_first(var, ".//name"))
      var_label <- xml_text(xml_find_first(var, ".//label"))
      field_size <- xml_text(xml_find_first(var, ".//fieldSize"))
      tag <- xml_text(xml_find_first(var, ".//tag"))
      var_filename <- xml_text(xml_find_first(var, ".//filename")) # Extract variable filename

      # Extract value labels (if available)
      value_labels <- xml_find_all(var, ".//valueLabelElement")
      value_label_list <- lapply(value_labels, function(value_label) {
        value <- xml_text(xml_find_first(value_label, ".//value"))
        label <- xml_text(xml_find_first(value_label, ".//label"))
        list(value = value, label = label)
      })

      list(
        name = var_name,
        label = var_label,
        fieldSize = field_size,
        tag = tag,
        filename = var_filename, # Add variable-level filename
        valueLabels = value_label_list
      )
    })

    # Store the entity and its variables (with filenames) in the result list
    result[[entity_name]] <- list(
      name = entity_name,
      label = entity_label,
      filename = entity_filename, # Entity-level filename
      tag = entity_tag,
      variables = variable_list
    )
  }

  return(result)
}
