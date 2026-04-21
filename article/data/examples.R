library(redatam)
library(dplyr)

fout <- "data/chl17_pop.rds"

if (!file.exists(fout)) {
chl17 <- read_redatam("downloads/redatam/CP2017CHL/BaseOrg16/CPV2017-16.dicx")

chl17_pop <- chl17$region %>%
  select(region_ref_id, nregion) %>%
  inner_join(
    chl17$provinci %>%
      select(provinci_ref_id, region_ref_id)
  ) %>%
  inner_join(
    chl17$comuna %>%
      select(comuna_ref_id, provinci_ref_id)
  ) %>%
  inner_join(
    chl17$distrito %>%
      select(distrito_ref_id, comuna_ref_id)
  ) %>%
  inner_join(
    chl17$area %>%
      select(area_ref_id, distrito_ref_id)
  ) %>%
  inner_join(
    chl17$zonaloc %>%
      select(zonaloc_ref_id, area_ref_id)
  ) %>%
  inner_join(
    chl17$vivienda %>%
      select(vivienda_ref_id, zonaloc_ref_id)
  ) %>%
  inner_join(
    chl17$hogar %>%
      select(hogar_ref_id, vivienda_ref_id)
  ) %>%
  inner_join(
    chl17$persona %>%
      select(persona_ref_id, hogar_ref_id, p08, p09)
  )

  saveRDS(chl17_pop, fout)
}
