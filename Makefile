# main: main.cpp branch_and_cut.cpp solution.o solveur.o client.o instance.o
# 	g++ -DIL_STD -I/opt/ibm/ILOG/CPLEX_Studio128/cplex/include -I/opt/ibm/ILOG/CPLEX_Studio128/concert/include -L/opt/ibm/ILOG/CPLEX_Studio128/cplex/lib/x86-64_linux/static_pic -L/opt/ibm/ILOG/CPLEX_Studio128/concert/lib/x86-64_linux/static_pic  -lconcert -lilocplex -lcplex -lm -lpthread -ldl -Wno-ignored-attributes solution.o solveur.o client.o instance.o -c branch_and_cut.cpp -c main.cpp -o main
# solveur.o: Solveur.cpp Solveur.h Solution.h #instance.o client.o
# 	g++ -DIL_STD -I/opt/ibm/ILOG/CPLEX_Studio128/cplex/include -I/opt/ibm/ILOG/CPLEX_Studio128/concert/include -L/opt/ibm/ILOG/CPLEX_Studio128/cplex/lib/x86-64_linux/static_pic -L/opt/ibm/ILOG/CPLEX_Studio128/concert/lib/x86-64_linux/static_pic  -lconcert -lilocplex -lcplex -lm -lpthread -ldl -Wno-ignored-attributes -c Solveur.cpp -o solveur.o 
# solution.o: Solution.cpp Solution.h Instance.h# instance.o
# 	g++ -c Solution.cpp -o solution.o 
# instance.o: Instance.cpp Instance.h # client.o
# 	g++ -c Instance.cpp -o instance.o
# client.o: Client.cpp Client.h
# 	g++ -c Client.cpp -o client.o
# clean: 
# 	rm -f *.o

main:
	g++ -DIL_STD -I/opt/ibm/ILOG/CPLEX_Studio128/cplex/include -I/opt/ibm/ILOG/CPLEX_Studio128/concert/include -L/opt/ibm/ILOG/CPLEX_Studio128/cplex/lib/x86-64_linux/static_pic -L/opt/ibm/ILOG/CPLEX_Studio128/concert/lib/x86-64_linux/static_pic -o main main.cpp Solveur.cpp branch_and_cut.cpp Solution.cpp Instance.cpp Client.cpp -lconcert -lilocplex -lcplex -lm -lpthread -ldl -Wno-ignored-attributes
