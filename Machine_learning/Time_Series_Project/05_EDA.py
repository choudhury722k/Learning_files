# 05 EDA 1

## Visualize the time series using line plot

def plot_df(x, y, title="", xlabel='Date', ylabel='Value', dpi=100):
    plt.figure(figsize=(16,5), dpi=dpi)
    plt.plot(x, y, color='tab:red')
    plt.gca().set(title=title, xlabel=xlabel, ylabel=ylabel)
    plt.show()

plot_df(x=df[input_date_variable], y=df[input_target_variable], title='Time Series Line Plot')

# Two side view line plot
x = df[input_date_variable]
y1 = df[input_target_variable].values
fig, ax = plt.subplots(1, 1, figsize=(16,5), dpi= 120)
plt.fill_between(x, y1=y1, y2=-y1, alpha=0.5, linewidth=2, color='seagreen')
plt.title('Time Series (Two Side View)', fontsize=16)
plt.hlines(y=0, xmin=np.min(df.date), xmax=np.max(df.date), linewidth=.5)
plt.show()


## Distribution of time series

sns.distplot(df[input_target_variable], kde = False, color ='red', bins = 20)
plt.title('Distribution of the Time Series', fontsize=16)
plt.show()


