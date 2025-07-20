# Drogon template

This is a template for a Drogon C++ web application. It includes a basic structure for building a RESTful API with database integration, JWT authentication, and more.

## Table of Contents

- [Prerequisites](#prerequisites)
- [Cloning the Repository](#cloning-the-repository)
- [Building the Project](#building-the-project)
- [Configuration](#configuration)
- [Running the Application](#running-the-application)

## Prerequisites

Before you begin, ensure you have the following installed:

- **C++ Compiler:** A C++20 compatible compiler.
- **CMake:** Version 3.10 or higher.
- **PostgreSQL:** A running PostgreSQL server.
- **Drogon:** The Drogon C++ web framework. Shipped as a submodule.
- **jwt-cpp:** The JWT (JSON Web Tokens) library for C++. Shipped as a submodule.
- **libpq-dev:** The PostgreSQL client library development headers.
- **uuid-dev:** The UUID library development headers.

## Cloning the Repository

1. Open your terminal and navigate to the directory where you want to clone the project.
2. Run the following command:

    ```bash
    git clone --recursive --depth=1 --shallow-submodules https://github.com/1Kuso4ek1/drogon-template.git
    cd drogon-template
    ```

    **Note:** The `--depth=1 --shallow-submodules` flags are used for faster cloning. If you need to work with the Drogon submodule directly (e.g., switch branches), you might need to clone without these flags.

## Building the Project

1. Create a build directory:

    ```bash
    mkdir build
    cd build
    ```

2. Configure the project using CMake:

    ```bash
    cmake .. -DCMAKE_BUILD_TYPE=Release
    ```

3. Build the project:

    ```bash
    make -j$(nproc)
    ```

## Configuration

There's a `config/config-template.json` file, which you should use as a template for your configuration. You can either make changes to this file directly, or copy/rename/move it. Either way, you need to fill in your database credentials, server port, and other settings.

## Running the Application

1. Navigate to the `build` directory:

    ```bash
    cd build
    ```

2. Run the executable:

    ```bash
    ./backend
    ```

    This will start the Drogon web server.

3. If you moved the default `config/config-template.json` file, you can provide a path to your config file as an argument:

    ```bash
    ./backend /path/to/your/config.json
    ```
