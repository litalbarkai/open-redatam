## R CMD check results

0 errors | 0 warnings | 0 note

* This update addresses the clang-ASAN and gcc-UBSAN warnings here
  https://cran.r-project.org/web/checks/check_results_redatam.html
* After running R with Valgrind locally (Ubuntu 20.04, gcc) I see the following
  results for my current changes

```
> valgrind::test_memory(".", "./dev/test-with-valgrind.R", leak_check='full', show_leak_kinds='all')
$leak_summary
[1] "definitely lost: 0 bytes in 0 blocks"              
[2] "indirectly lost: 0 bytes in 0 blocks"              
[3] "possibly lost: 0 bytes in 0 blocks"                
[4] "still reachable: 65,021,713 bytes in 13,449 blocks"
[5] "of which reachable via heuristic:"                 
[6] "newarray           : 4,264 bytes in 1 blocks"      
[7] "suppressed: 0 bytes in 0 blocks"                   

$error_summary
[1] "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)"
```
