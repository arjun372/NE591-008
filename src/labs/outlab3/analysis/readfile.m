clear all;
close all;
format longEng

    % Specify the file name filename = 'discontinous.csv';

myfunction = @(x) x - x + 3.60734;

% Read the table from the CSV file data = readtable(filename);

% Extract the required columns m = data.m;

points = data.m;
integral( :, 1) = data.trapezoidal;
integral( :, 2) = data.simpsons;
integral( :, 3) = data.gauss_legendre;

runtime( :, 1) = data.trapezoidalRuntime_ns_;
runtime( :, 2) = data.SimpsonRuntime_ns_;
runtime( :, 3) = data.gaussRuntimeNs;

runtime = runtime./ 1e3;
error = abs(myfunction(1) - integral);

F = figure;
hold on;

set(gca, 'Fontsize', [18]);
set(gca, 'FontName', 'cmr12');
set(gca, "TickLabelInterpreter", "latex");
set(gca, 'Xscale', 'log');
set(gca, 'Yscale', 'log');

grid on grid minor box on % Plot f(x)
as a smooth line plot(points, error, '--');
set(gca, 'ColorOrderIndex', 1) scatter(points, error, 'LineWidth', 2);

legend('Trapezoidal Rule', "Simpson's Rule", 'Gauss-Legendre Quadrature', 'Interpreter', 'latex', 'Location',
       'northeast');

xticks((points));

title("Absolute error vs sampling intervals", 'Interpreter', 'latex');
xlabel('$m$, $m=2^{n}$ for Gauss-Legendre Quadrature', 'Interpreter', 'latex');
ylabel('Absolute Error', 'Interpreter', 'latex');

% xlim([ 1, 2048 ])

        pos = get(gcf, 'Position');
% // gives x left, y bottom, width, height
    pos(3) = 1024;
pos(4) = 420;
set(gcf, 'Position', pos);
% print(gcf, strcat(filename, '.error.png'), '-dpng', '-r600');
% exportgraphics(gcf, strcat(filename, '.png'), 'BackgroundColor', 'white', 'ContentType', 'image', 'Resolution', 600);
hold off;

F = figure;
hold on;

set(gca, 'Fontsize', [18]);
set(gca, 'FontName', 'cmr12');
set(gca, "TickLabelInterpreter", "latex");
set(gca, 'Xscale', 'log');
set(gca, 'Yscale', 'log');

grid on grid minor box on % Plot f(x)
as a smooth line plot(points, runtime, '--');
set(gca, 'ColorOrderIndex', 1) scatter(points, runtime, 'LineWidth', 2);

legend('Trapezoidal Rule', "Simpson's Rule", 'Gauss-Legendre Quadrature', 'Interpreter', 'latex', 'Location',
       'northwest');

xticks((points));

title("Runtime [$\mu$s] vs sampling intervals", 'Interpreter', 'latex');
xlabel('$m$, $m=2^{n}$ for Gauss-Legendre Quadrature', 'Interpreter', 'latex');
ylabel('Runtime [$\mu$s]', 'Interpreter', 'latex');

% xlim([ 1, 2048 ])

        pos = get(gcf, 'Position');
% // gives x left, y bottom, width, height
    pos(3) = 1024;
pos(4) = 420;
set(gcf, 'Position', pos);
% print(gcf, strcat(filename, '.runtime.png'), '-dpng', '-r600');
% exportgraphics(gcf, strcat(filename, '.png'), 'BackgroundColor', 'white', 'ContentType', 'image', 'Resolution', 600);
hold off;
