## R CMD check results

0 errors | 0 warnings | 2 note

❯ checking CRAN incoming feasibility ... [5s/15s] NOTE
  Maintainer: ‘Mauricio Vargas Sepulveda <m.sepulveda@mail.utoronto.ca>’
  
  New submission
  
  Package was archived on CRAN
  
  CRAN repository db overrides:
    X-CRAN-Comment: Archived on 2024-11-06 for repeated policy violation.
  
    Repeatedy spamming a team member's personal email address in HTML.

❯ checking compilation flags used ... NOTE
  Compilation used the following non-portable flag(s):
    ‘-Wp,-D_FORTIFY_SOURCE=3’
    
Fixes: 

* This was tested with all available R-Hub images including the Clang ASAN image.
* Fixes UBSan issues (https://win-builder.r-project.org/incoming_pretest/redatam_2.0.3_20241107_154750/specialChecks/clang-san/summary.txt)
* Fixes the empty string issue (https://www.stats.ox.ac.uk/pub/bdr/memtests/clang-ASAN/redatam/00check.log)
* Uses 2 threads during R CMD check
