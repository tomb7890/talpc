FROM kost13/cpp-gtest:latest

# Set the working directory inside the container (adjust as needed)
WORKDIR /app

# Copy project's source code into the container
COPY . /app

# Build project 
RUN make