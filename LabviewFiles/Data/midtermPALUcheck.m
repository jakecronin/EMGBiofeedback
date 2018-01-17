L = [1 0 0 0; 
     0.5 1 0 0;
    -0.25 0.5 1 0;
    -0.5 -0.5 -0.5 1];

Lsean = [1 0 0 0;
        0.5 1 0 0;
        0.25 -0.5 1 0;
        -0.5 0.5 0.5 1];
    
    

Lmine = [1 0 0 0; 
     -0.5 1 0 0;
    0.25 -0.5 1 0;
    0.5 0.5 0.5 1];


U = [4 0 2 -2;
     0 4 3 3;
     0 0 3 4;
     0 0 0 -2.5];

P = [1 0 0 0; 
     0 0 0 1; 
     0 0 1 0; 
     0 1 0 0];

A = L * U;
A = P' * A;

Amine = Lmine * U;
Amine = P' * Amine;

Asean = Lsean * U;
Asean = P' * Asean;

disp(A);
disp(Amine);
disp(Asean);

%[Li, Ui] = lu(A);

%disp(Li)
%disp(Ui)