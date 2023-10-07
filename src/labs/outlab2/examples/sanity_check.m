% Define the parametric function f(x) = x ^ 4 - x ^ 3 - x ^ 2 + x + 1 f = @(x) x.^ 4 - x.^ 3 - x.^ 2 + x + 1;

% Define the range of x values you want to plot over xmin = -1.5;
% Minimum x value xmax = 1.5;  % Maximum x value

% Number of points for the smooth curve
num_points = 1000;

% Generate a vector of x values for the smooth curve
x_smooth = linspace(xmin, xmax, num_points);

% Evaluate the function f(x)
at the smooth curve points y_smooth = f(x_smooth);

% Number of points to sample n = 10;

% Generate n random x values in the specified range x_samples = xmin + (xmax - xmin) * rand(1, n);

% Evaluate the function f(x)
at the sampled x values y_samples = f(x_samples);

% Display the sampled points (x, f(x))
disp([x_samples; y_samples]');



% Plot the smooth curve
plot(x_smooth, y_smooth, 'LineWidth', 2, 'DisplayName', 'Smooth Curve');
hold on;
% Plot the sampled points
scatter(x_samples, y_samples, 'filled', 'DisplayName', 'Sampled Points');

xlabel('x');
ylabel('f(x)');
title('Sampled Points and Smooth Curve of f(x) = x^4 - x^3 - x^2 + x + 1');
grid on;
legend;
hold off;
