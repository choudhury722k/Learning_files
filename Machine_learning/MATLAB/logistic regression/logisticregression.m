clear ; close all; clc
%% 1: Plot
data = load('logisticregressiondata.txt');
X = data(:, [1, 2]); 
y = data(:, 3);
fprintf(['Plotting data with + indicating (y = 1) examples and o ' ...
         'indicating (y = 0) examples.\n']);
plotData(X, y);
xlabel('Exam 1 score')
ylabel('Exam 2 score')
legend('Admitted', 'Not admitted')
hold off;
fprintf('\nProgram paused. Press enter to continue.\n');
pause;


%% 2: calculating cost function using some assumed theta
[m, n] = size(X);
X = [ones(m, 1) X];
initial_theta = zeros(n + 1, 1);
[cost, grad] = costFunction(initial_theta, X, y);
fprintf('Cost at initial theta (zeros): %f\n', cost);
fprintf('Gradient at initial theta (zeros): \n');
fprintf(' %f \n', grad);

test_theta = [-24; 0.2; 0.2];
[cost, grad] = costFunction(test_theta, X, y);
fprintf('\nCost at test theta: %f\n', cost);
fprintf('Gradient at test theta: \n');
fprintf(' %f \n', grad);
fprintf('\nProgram paused. Press enter to continue.\n');
pause;


%% 3: Optimizing using fminunc
options = optimset('GradObj', 'on', 'MaxIter', 400);
%  Run fminunc to obtain the optimal theta
%  This function will return theta and the cost 
[theta, cost] = ...
	fminunc(@(t)(costFunction(t, X, y)), initial_theta, options);
%fminunc is an optimization function; it takes a function handle as input 
%and returns an optimal vector, which minimizes the function.
%The function handle should have this property: 
%It should be accepted only a single vector, and output a scalar.

fprintf('Cost at theta found by fminunc: %f\n', cost);
fprintf('theta: \n');
fprintf(' %f \n', theta);

 
%% 4: Plot Decision Boundary
plotDecisionBoundary(theta, X, y);
hold on;
xlabel('Exam 1 score')
ylabel('Exam 2 score')
legend('Admitted', 'Not admitted')
hold off;
fprintf('\nProgram paused. Press enter to continue.\n');
pause;


%% 5: Accuracy
prob = sigmoid([1 45 85] * theta);
fprintf(['For a student with scores 45 and 85, we predict an admission ' ...
         'probability of %f\n'], prob);
% Compute accuracy on our training set
m = size(X, 1);
p = zeros(m, 1);
p=sigmoid(X*theta)>=0.5;
fprintf('Train Accuracy: %f\n', mean(double(p == y)) * 100);
fprintf('\n');

fprintf('\nThank you :)\n');


%% ================================================================= %%


%% Cost Function and Gradient calculation
function [J, grad] = costFunction(theta, X, y)
m = length(y); 
J = 0;
grad = zeros(size(theta));
h=sigmoid(X*theta);
J=(1/m)*((-y' * log(h)) - (1-y)' * log(1-h));
grad=(1/m)*(h-y)' * X;
end


%% Sigmoid Function calculation
function g = sigmoid(z)
g = zeros(size(z));
g=1./(1+exp(-z));
end


%% Ploting Function
function plotData(X, y)
figure; 
hold on;
% helps in finding index of 1 and 0 for plotting
pos=find(y==1);   
neg=find(y==0);
plot(X(pos, 1), X(pos, 2), 'k+','LineWidth', 2, 'MarkerSize', 7);
plot(X(neg, 1), X(neg, 2), 'ko','MarkerFaceColor', 'y', 'MarkerSize', 7);
hold off;
end


%% Decision Boundary Function
function plotDecisionBoundary(theta, X, y)
%   PLOTDECISIONBOUNDARY(theta, X,y) plots the data points with + for the 
%   positive examples and o for the negative examples. X is assumed to be 
%   a either 
%   1) Mx3 matrix, where the first column is an all-ones column for the 
%      intercept.
%   2) MxN, N>3 matrix, where the first column is all-ones

plotData(X(:,2:3), y);
hold on
if size(X, 2) <= 3
    % Only need 2 points to define a line, so choose two endpoints
    plot_x = [min(X(:,2))-2,  max(X(:,2))+2];
    % Calculate the decision boundary line
    plot_y = (-1./theta(3)).*(theta(2).*plot_x + theta(1));
    % Plot, and adjust axes for better viewing
    plot(plot_x, plot_y)
    % Legend, specific for the exercise
    legend('Admitted', 'Not admitted', 'Decision Boundary')
    axis([30, 100, 30, 100])
else
    % Here is the grid range
    u = linspace(-1, 1.5, 50);
    v = linspace(-1, 1.5, 50);
    z = zeros(length(u), length(v));
    % Evaluate z = theta*x over the grid
    for i = 1:length(u)
        for j = 1:length(v)
            z(i,j) = mapFeature(u(i), v(j))*theta;
        end
    end
    z = z'; % important to transpose z before calling contour
    % Plot z = 0
    % Notice you need to specify the range [0, 0]
    contour(u, v, z, [0, 0], 'LineWidth', 2)
end
hold off
end


%% IMPORTANT FORMULAS
% Simoid function
% h=1./(1+exp(-z))
% Cost function
% J=(1/m)*((-y' * log(h)) - (1-y)' * log(1-h));
% Gradient
% grad=(1/m)*(h-y)' * X;