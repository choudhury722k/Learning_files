data = load('linearregmultidata.txt');
X = data(:, 1:2);
y = data(:, 3);
m = length(y);
fprintf('dataset: \n');
fprintf(' x = [%.0f %.0f], y = %.0f \n', [X(1:end,:) y(1:end,:)]');

fprintf('Program paused. Press enter to continue.\n');
pause;

% Scale features and set them to zero mean
fprintf('Normalizing Features ...\n');
X_norm = X;
mu = zeros(1, size(X, 2));
sigma = zeros(1, size(X, 2));
mu = mean(X);
sigma = std(X);
X_norm = (X - mu)./sigma;
Xn = [ones(m, 1) X_norm]


% calculating theta gradient descent
fprintf('Running gradient descent ...\n');
alpha = 0.01;
num_iters = 400;
theta = zeros(3, 1);
J_history = zeros(num_iters, 1);
for iter = 1:num_iters
delta= (Xn * theta) - y;
theta=theta-((alpha/m)*Xn'*delta);
J_history(iter) = computeCostMulti(Xn, y, theta);
end

figure;
plot(1:numel(J_history), J_history, '-b', 'LineWidth', 2);
xlabel('Number of iterations');
ylabel('Cost J');
% numel return no of arrays elements in a n array

fprintf('Theta computed from gradient descent: \n');
fprintf(' %f \n', theta);
fprintf('\n');
price = sum(Xn*theta);
fprintf(['Predicted price of a 1650 sq-ft, 3 br house ' ...
         '(using gradient descent):\n $%f\n'], price);

fprintf('Program paused. Press enter to continue.\n');
pause;


% calculating theta by normal equation
fprintf('Solving with normal equations...\n');
data = csvread('linearregmultidata.txt');
X = data(:, 1:2);
y = data(:, 3);
m = length(y);
X = [ones(m, 1) X];
theta = zeros(size(X, 2), 1);
theta=pinv(X'*X)*X'*y;
% pinv forms matrix inverse

fprintf('Theta computed from the normal equations: \n');
fprintf(' %f \n', theta);
fprintf('\n');
price = sum(X*theta);
fprintf(['Predicted price of a 1650 sq-ft, 3 br house ' ...
         '(using normal equations):\n $%f\n'], price);

     
     
function J = computeCostMulti(X, y, theta)
m = length(y);
J = 0;
h=X*theta;
sq=(h-y).^2;
J=1/(2*m)*sum(sq);
end

