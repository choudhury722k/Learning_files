clear ; close all; clc                        
%% 1: Loading and Visualizing Data
fprintf('Loading and Visualizing Data ...\n')
load('sampledata.mat');
input_layer_size  = 400;  % 20x20 Input Images of Digits
num_labels = 10;          % 10 labels, from 1 to 10
m = size(X, 1);
% Randomly select 100 data points to display
rand_indices = randperm(m);
sel = X(rand_indices(1:100), :);
displayData(sel);
fprintf('Program paused. Press enter to continue.\n');
pause;


%% 2: Vectorize Logistic Regression
fprintf('\nTesting lrCostFunction() with regularization');
theta_t = [-2; -1; 1; 2];
X_t = [ones(5,1) reshape(1:15,5,3)/10];
y_t = ([1;0;1;0;1] >= 0.5);
lambda_t = 3;
[J , grad] = lrCostFunction(theta_t, X_t, y_t, lambda_t);
fprintf('\nCost: %f\n', J);
fprintf('Gradients:\n');
fprintf(' %f \n', grad);
fprintf('Program paused. Press enter to continue.\n');
pause;


%% 3: one vs all
fprintf('\nTraining One-vs-All Logistic Regression...\n')
lambda = 0.1;
[all_theta] = oneVsAll(X, y, num_labels, lambda);
fprintf('Program paused. Press enter to continue.\n');
pause;


%% 4: Predict for One-Vs-All
pred = predictOneVsAll(all_theta, X);
fprintf('\nTraining Set Accuracy: %f\n', mean(double(pred == y)) * 100);

fprintf('\nThank you :)\n');


%% ================================================================= %%


%% Display Data function
function [h, display_array] = displayData(X, example_width)
%DISPLAYDATA Display 2D data in a nice grid
%   stored in X in a nice grid. It returns the figure handle h and the 
% Set example_width automatically if not passed in
if ~exist('example_width', 'var') || isempty(example_width) 
	example_width = round(sqrt(size(X, 2)));
end

% Gray Image
colormap(gray);
% Compute rows, cols
[m n] = size(X);
example_height = (n / example_width);
% Compute number of items to display
display_rows = floor(sqrt(m));
display_cols = ceil(m / display_rows);
% Between images padding
pad = 1;
% Setup blank display
display_array = - ones(pad + display_rows * (example_height + pad), ...
                       pad + display_cols * (example_width + pad));
% Copy each example into a patch on the display array
curr_ex = 1;
for j = 1:display_rows
	for i = 1:display_cols
		if curr_ex > m, 
			break; 
		end
		% Copy the patch
		% Get the max value of the patch
		max_val = max(abs(X(curr_ex, :)));
		display_array(pad + (j - 1) * (example_height + pad) + (1:example_height), ...
		              pad + (i - 1) * (example_width + pad) + (1:example_width)) = ...
						reshape(X(curr_ex, :), example_height, example_width) / max_val;
		curr_ex = curr_ex + 1;
	end
	if curr_ex > m, 
		break; 
	end
end
h = imagesc(display_array, [-1 1]);
axis image off
drawnow;
end


%% Cost Function Calculation
function [J, grad] = lrCostFunction(theta, X, y, lambda)
m = length(y); 
J = 0;
grad = zeros(size(theta));
v1=log(sigmoid(X*theta));
v2=log(1-sigmoid(X*theta));
J= -1/m*((y'*v1)+((1-y)'*v2))+((lambda/(2*m))*sum(theta(2:size(theta)).^2));
grad(1)= 1/m*(X(:,1)'*(sigmoid(X*theta)-y));
grad(2:end)= 1/m*(X(:,2:end)'*(sigmoid(X*theta)-y))+(lambda/m)*theta(2:end);
grad = grad(:);
end


%% Sigmoid function
function g = sigmoid(z)
g = 1.0 ./ (1.0 + exp(-z));
end


%% One vs all function
function [all_theta] = oneVsAll(X, y, num_labels, lambda)
%ONEVSALL trains multiple logistic regression classifiers and returns all
%the classifiers in a matrix all_theta, where the i-th row of all_theta 
%corresponds to the classifier for label i
%   [all_theta] = ONEVSALL(X, y, num_labels, lambda) trains num_labels
%   logistic regression classifiers and returns each of these classifiers
%   in a matrix all_theta, where the i-th row of all_theta corresponds 
%   to the classifier for label i

m = size(X, 1);
n = size(X, 2);
all_theta = zeros(num_labels, n + 1);
X = [ones(m, 1) X];
 
% Hint: theta(:) will return a column vector.
%
% Hint: You can use y == c to obtain a vector of 1's and 0's that tell you
%       whether the ground truth is true/false for this class.
%       fmincg works similarly to fminunc, but is more efficient when we
%       are dealing with large number of parameters.
%
% Example Code for fmincg:
%
%     % Set Initial theta
%     initial_theta = zeros(n + 1, 1);
%     % Set options for fminunc
%     options = optimset('GradObj', 'on', 'MaxIter', 50);
%     % Run fmincg to obtain the optimal theta
%     % This function will return theta and the cost 
%     [theta] = ...
%         fmincg (@(t)(lrCostFunction(t, X, (y == c), lambda)), ...
%                 initial_theta, options);
%
initial_theta = zeros(n + 1, 1);
options = optimset('GradObj', 'on', 'MaxIter', 50);
for i = 1:num_labels
    c=i*ones(size(y));  
    [theta] = fmincg (@(t)(lrCostFunction(t, X, (y == c), lambda)),initial_theta, options);
    all_theta(i,:)=theta(:);
end
end


%% 
function p = predictOneVsAll(all_theta, X)
%PREDICT Predict the label for a trained one-vs-all classifier. The labels 
%are in the range 1..K, where K = size(all_theta, 1). 
%  p = PREDICTONEVSALL(all_theta, X) will return a vector of predictions
%  for each example in the matrix X. Note that X contains the examples in
%  rows. all_theta is a matrix where the i-th row is a trained logistic
%  regression theta vector for the i-th class. You should set p to a vector
%  of values from 1..K (e.g., p = [1; 3; 1; 2] predicts classes 1, 3, 1, 2
%  for 4 examples) 

m = size(X, 1);
num_labels = size(all_theta, 1);
p = zeros(size(X, 1), 1);
X = [ones(m, 1) X];
      
z=X*all_theta';
h=sigmoid(z);
[~, p]=max(h,[],2);
end
