devtools::load_all("./rpkg")

dout <- "dev/redatam-microdata"

if (!dir.exists(dout)) dir.create(dout)

finp <- list.files("downloads/redatam", pattern = "\\.dic", full.names = TRUE, recursive = TRUE)

finp <- finp[!finp %in%
  c("downloads/redatam/ECI2015MEX/BaseR/ECI2015MX.dicX",
  "downloads/redatam/CP2000BRA/BaseOriginal/CD102_PUBLICO.dic",
  "downloads/redatam/CP2001BOL/Cp2001BOL/BaseOriginal/CPV2001.dic",
  "downloads/redatam/CP2010MEX/BasePubM/MC10.dic")
]

finp <- finp[!grepl("BRA", finp)]
finp <- finp[!grepl("Celade|celade", finp)]

purrr::map(
  finp,
  function(f) {
    print(f)

    dout2 <- gsub(basename(f), "", f)
    dout2 <- gsub("downloads/redatam", dout, dout2)

    if (!dir.exists(dout2)) {
      dir.create(dout2, recursive = TRUE)
    } else {
      return(TRUE)
    }

    d <- read_redatam(f)

    fout <- paste0(dout2, basename(dout2), ".rds")

    saveRDS(d, fout)

    dout3 <- paste0(dout2, "csv")

    if (!dir.exists(dout3)) {
      dir.create(dout3)
    }

    # save each element of the list as a csv file
    purrr::map(
      names(d),
      function(n) {
        fout <- paste0(dout3, "/", n, ".csv")
        readr::write_delim(d[[n]], fout, delim = ";")
      }
    )

    # put all the csv files into a zip file
    fout <- paste0(dout2, basename(dout2), ".zip")

    finp <- list.files(dout3, pattern = "\\.csv", full.names = TRUE)

    # use 7z + system to add all the csv files to a zip file
    system(paste0("7z a -tzip -r ", fout, " ./", paste(finp, collapse = " ./")))

    unlink(dout3, recursive = TRUE)

    return(TRUE)
  }
)
