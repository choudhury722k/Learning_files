# 08 ACF-PACF plots and Find the order of differencing

# Original Series
fig, axes = plt.subplots(3, 3)
axes[0, 0].plot(df[input_target_variable]); axes[0, 0].set_title('Original Series')
plot_acf(df[input_target_variable], ax=axes[0, 1])
plot_pacf(df[input_target_variable], lags=50, ax=axes[0, 2])

# 1st Differencing
axes[1, 0].plot(df[input_target_variable].diff()); axes[1, 0].set_title('1st Order Differencing')
plot_acf(df[input_target_variable].diff().dropna(), ax=axes[1, 1])
plot_pacf(df[input_target_variable].diff().dropna(), ax=axes[1, 2])


# 2nd Differencing
axes[2, 0].plot(df[input_target_variable].diff().diff()); axes[2, 0].set_title('2nd Order Differencing')
plot_acf(df[input_target_variable].diff().diff().dropna(), ax=axes[2, 1])
plot_pacf(df[input_target_variable].diff().diff().dropna(), ax=axes[2, 2])

plt.show()

## ADF Test for 2 order of differencing
result = adfuller(df[input_target_variable].diff().diff().dropna().values, autolag='AIC')
print(f'ADF Statistic: {result[0]}')
print(f'p-value: {result[1]}')
for key, value in result[4].items():
    print('Critial Values:')
    print(f'   {key}, {value}')
