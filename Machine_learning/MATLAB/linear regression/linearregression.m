% plotting data
fprintf('Plotting Data ...\n')
data = load('data.txt');
X = data(:, 1);
y = data(:, 2);
% fprintf('\n X \n');
% fprintf('%f\n', X);
% fprintf('\n y \n');
% fprintf('%f\n', y);
figure;
plot(X, y, 'rx', 'MarkerSize', 10);
ylabel('Profit in $10,000s');
xlabel('Population of City in 10,000s');

fprintf('\nProgram paused. Press enter to continue.\n');
pause;

% initializing fitting parameters
m = length(y);
X = [ones(m, 1), data(:,1)]; 
theta = zeros(2, 1);
iterations = 1500;
alpha = 0.01;

% calculating theta by gradient descent
for iter = 1:iterations
delta=1/m*(X'*X*theta-X'*y);
theta=theta-alpha.*delta;
end

% print theta to screen
fprintf('\nTheta found by gradient descent:\n');
fprintf('%f\n', theta);

% Plot the linear fit
hold on; 
plot(X(:,2), X*theta, '-')
legend('Training data', 'Linear regression')
hold off 

fprintf('\nProgram paused. Press enter to continue.\n');
pause;

% Predict values for population sizes of 35,000 and 70,000
predict1 = [1, 3.5] *theta;
fprintf('\nFor population = 35,000, we predict a profit of %f\n',...
    predict1*10000);
predict2 = [1, 7] * theta;
fprintf('\nFor population = 70,000, we predict a profit of %f\n',...
    predict2*10000);

fprintf('\nThank you :) \n');


% COST FUNCTION
% h=X*theta;
% J=1/(2*m)*sum(((h-y).^2));
% GRADIENT DESCEND 
% {
% delta=1/m*(X'*X*theta-X'*y);
% theta=theta-alpha.*delta;
% }
% delta=d/dx(J);