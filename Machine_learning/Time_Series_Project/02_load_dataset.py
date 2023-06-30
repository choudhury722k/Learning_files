# 02 Load Dataset 

# Read data in form of a csv file
df = pd.read_csv(input_file_name)

df[input_date_variable] = pd.to_datetime(df[input_date_variable])

# First 5 rows of the dataset
df.head()
