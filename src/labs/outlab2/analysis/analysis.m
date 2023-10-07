
read_output_columns =
    @(pathname) deal(readtable(pathname).('xi'), readtable(pathname).('Lxi'),
                     readtable(pathname).('fxi'), readtable(pathname).('Exi'));
read_input_columns = @(pathname) deal(readtable(pathname).('x'));

% % n = 3 [x] = read_input_columns('n3.csv');
[ xi, L_xi, f_xi, E_xi ] = read_output_columns('output_n3.csv');
n = size(x, 1);
m = size(xi, 1);
plot_points(n, m, x, xi, L_xi, f_xi, E_xi);

% % n = 4 [x] = read_input_columns('n4.csv');
[ xi, L_xi, f_xi, E_xi ] = read_output_columns('output_n4.csv');
n = size(x, 1);
m = size(xi, 1);
plot_points(n, m, x, xi, L_xi, f_xi, E_xi);

% % n = 8 [x] = read_input_columns('n8.csv');
[ xi, L_xi, f_xi, E_xi ] = read_output_columns('output_n8.csv');
n = size(x, 1);
m = size(xi, 1);
plot_points(n, m, x, xi, L_xi, f_xi, E_xi);
