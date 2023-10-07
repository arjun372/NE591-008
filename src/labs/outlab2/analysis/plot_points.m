function[F] = plot_points(n, m, x, xi, L_xi, f_xi, E_xi) F = figure;
hold on;

set(gca, 'Fontsize', [18]);
set(gca, 'FontName', 'cmr12');
set(gca, "TickLabelInterpreter", "latex");
% set(gca, 'Xscale', xscale);

grid on grid minor box on % Plot f(x)
as a smooth line plot(xi, f_xi, 'LineWidth', 2);
% Plot L(x)
as points with E(x)
as the error bars errorbar(xi, L_xi, E_xi, ".", 'LineWidth', 0.75, 'Color', 'k');
scatter(xi, L_xi, 50, 'red');

% Define the function f = @(x) exp(x);

% Define the x values x = linspace(-1, 1, m);

% Calculate the function values y = f(x);

% Calculate the nth derivative
for i = 1:n
    y = diff(y) ./ diff(x);
x = x(1 : end - 1);
% adjust the x values end

    % % Define the function and its derivatives % f = @(x) exp(x);
% df = @(x) exp(x);
% derivative of e ^ x is e ^ x % ddf = @(x) exp(x);
% second derivative of e ^ x is e ^ x % dddf = @(x) exp(x);
% third derivative of e ^ x is e ^ x % ddddf = @(x) exp(x);
% fourth derivative of e ^ x is e ^ x % % for (i = 0; i <= n; i++) % Define the nodes x_nodes = linspace(-1, 1, n);
% change this to your actual nodes % Compute the maximum of the fourth derivative in the interval[-1, 1] z =
    xi'; % linspace(-1, 1, m); max_ddddf = max(y);
% Compute the product term
product_term = prod(repmat(z', 1, length(x_nodes)) - repmat(x_nodes, length(z), 1), 2);
% Compute the error bound
error_bound = abs(max_ddddf / factorial(n)) * max(abs(product_term));

plot(xi, L_xi + error_bound, '--');
plot(xi, L_xi - error_bound, '--');
disp(['The estimated error bound for the Lagrange interpolation is ', num2str(error_bound)]);


% Add labels and title
xlabel('$x_i$', 'Interpreter','latex');
ylabel('$f(x_i)$, $L(x_i)$','Interpreter','latex');

% Create the title
title_str = ['n = ', num2str(n), ', m = ',num2str(m),', $f(x) = e^{x}$'];

% Set the title
title(title_str, 'Interpreter','latex');

% title('n, m, $f(x) = e^{x}$', 'Interpreter','latex');
error_str = ['Error Bound = ', num2str(error_bound)];

% Add a legend
legend('$f(x_i)$', '$E(x_i) = |L(x_i)-f(x_i)|$', '$L(x_i)$', error_str, 'Interpreter','latex', 'Location', 'northwest');

    pos = get(gcf, 'Position'); %// gives x left, y bottom, width, height
    pos(3) = 640;
    pos(4) = 768;
    set(gcf, 'Position', pos)

       % title(strcat('Figure ', int2str(fig_count)));
        % print(gcf,strcat('F', strcat(int2str(n), '.png')),'-dpng','-r600');

% Hold off the plot
hold off;
end
