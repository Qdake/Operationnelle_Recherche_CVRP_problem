Pour compiler:
    $ make
Pour tester avec une instance:
    $ ./main method_name instance_path


Vous pouvez utiliser toutes les instances dans le repertoire 'instance'.
Par ex:
    ./instance/Vrp-Set-P/P/P-n16-k8.vrp
    ./instance/Vrp-Set-P/P/P-n19-k2.vrp
    ./instance/Vrp-Set-P/P/P-n22-k8.vrp

Vous avez le choix entre trois methodes:
    branch_and_cut
    MTZ
    bin_plne_glouton


EX: 
    ./main branch_and_cut ./instance/Vrp-Set-P/P/P-n16-k8.vrp
    ./main MTZ ./instance/Vrp-Set-P/P/P-n16-k8.vrp
    ./main bin_plne_glouton ./instance/Vrp-Set-P/P/P-n16-k8.vrp