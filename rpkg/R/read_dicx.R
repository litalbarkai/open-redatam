get_variable_type <- function(type) {
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

  result[[root_name]] <- list(
    name = root_name,
    filename = xml_text(xml_find_first(root, ".//filename"))
  )

  result[[root_name]]$filename <- gsub(
    "\\.\\\\",
    paste0(dirname(filepath), "/"), result[[root_name]]$filename
  )
  result[[root_name]]$filename <- gsub("//", "/", result[[root_name]]$filename)

  variable <- xml_find_first(dicx, ".//variable")

  variable_filename <- gsub(
    "//", "/",
    gsub(
      "\\.\\\\",
      paste0(dirname(filepath), "/"),
      xml_text(xml_find_first(variable, ".//filename"))
    )
  )

  result[[root_name]][["variables"]] <- list(
    name = xml_text(xml_find_first(variable, ".//name")),
    label = xml_text(xml_find_first(variable, ".//label")),
    type = xml_text(xml_find_first(variable, ".//varType")),
    fieldsize = as.integer(
      xml_text(xml_find_first(variable, ".//fieldSize"))),
    # decimals = as.integer(
    #   xml_text(xml_find_first(variable, ".//decimals"))),
    range = as.integer(
      xml_text(xml_find_first(variable, ".//range"))),
    filename = variable_filename
  )

  entities <- xml_find_all(dicx, ".//entity")

  for (entity in entities) {
    entity_name <- xml_text(xml_find_first(entity, ".//name"))
    entity_label <- xml_text(xml_find_first(entity, ".//label"))

    entity_filename <- gsub(
      "//", "/",
      gsub(
        "\\.\\\\",
        paste0(dirname(filepath), "/"),
        xml_text(xml_find_first(entity, ".//filename"))
      )
    )

    variable_list <- lapply(
      xml_find_all(entity, ".//variable"),
      function(variable) {
        variable_type <- get_variable_type(
          xml_text(
            xml_find_first(
              xml_find_first(variable, ".//varDicChoice"), ".//datasetType"
            )
          )
        )

        variable_filename <- gsub(
          "//", "/",
          gsub(
            "\\.\\\\",
            paste0(dirname(filepath), "/"),
            xml_text(xml_find_first(variable, ".//filename"))
          )
        )

        value_label_list <- lapply(
          xml_find_all(variable, ".//valueLabelElement"),
          function(value_label) {
            list(
              value = xml_text(xml_find_first(value_label, ".//value")),
              label = xml_text(xml_find_first(value_label, ".//label"))
            )
          }
        )

        list(
          name = xml_text(xml_find_first(variable, ".//name")),
          label = xml_text(xml_find_first(variable, ".//label")),
          type = get_variable_type(
            xml_text(
              xml_find_first(
                xml_find_first(variable, ".//varDicChoice"), ".//datasetType"
              )
            )
          ),
          fieldsize = as.integer(
            xml_text(xml_find_first(variable, ".//fieldSize"))),
          filename = variable_filename,
          valuelabels = value_label_list
        )
      }
    )

    names(variable_list) <- sapply(variable_list, function(x) x$name)

    result[[entity_name]] <- list(
      name = entity_name,
      label = entity_label,
      filename = entity_filename,
      variables = variable_list
    )
  }

  return(result)
}
