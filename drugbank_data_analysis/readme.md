# DrugBank XML Data Analysis

## Project Description
This project is a comprehensive Python-based Data Engineering and Bioinformatics analysis tool built around the **DrugBank** database. It demonstrates the ability to extract, transform, load (ETL), and analyze complex, deeply nested XML biological data, ultimately exposing the findings via an interactive REST API. 

The application parses pharmacological, chemical, and genomic data to generate statistical insights, bipartite network graphs, and interactive dataframes.

## Key Features & Implementations

### 1. Advanced XML Parsing & ETL (Pandas, lxml)
* **Namespace Handling:** Efficiently parses large XML structures using `lxml` while stripping problematic namespaces for streamlined querying.
* **Data Extraction:** Generates structured `pandas.DataFrame` objects containing comprehensive drug profiles, including descriptions, indications, mechanisms of action, target proteins (polypeptides), dosages, and food/drug interactions.

### 2. Network Analysis & Visualization (NetworkX, Matplotlib)
* **Synonym Networks:** Generates graphical representations mapping unique DrugBank IDs to their known chemical synonyms.
* **Pathway-Drug Bipartite Graphs:** Visualizes complex interactions between biological/metabolic pathways and various pharmaceutical agents using color-coded bipartite graphs.
* **Gene-Drug-Product Trees:** A custom visualization mapping specific genes to the drugs that target them, and subsequently to the commercial pharmaceutical products containing those drugs.

### 3. Statistical Analysis & Plotting
* **Target Cellular Locations:** Analyzes polypeptide targets and visualizes their distribution across different cellular locations using pie charts.
* **Approval Statuses:** Tracks and charts the lifecycle of drugs (Approved, Withdrawn, Experimental, Vet-approved).
* **Cross-Database Analysis:** Compares internal DrugBank pathway data with external databases (SMPDB) to analyze enzyme counts per metabolic pathway.

### 4. Synthetic Data Generation
* **XML Simulator:** Includes a custom data generator capable of expanding the initial subset (100 drugs) to a massive mock database (e.g., 20,000 records) by randomizing and deep-copying existing XML nodes while assigning valid, sequential IDs and simulated timestamps.

### 5. Backend API (FastAPI & Uvicorn)
* **RESTful Endpoints:** Implements a localized server using `FastAPI` with asynchronous request handling.
* **Data Serving:** Features a `POST /drug_interaction/` endpoint that accepts a Drug ID payload and instantly returns the total count of interacting biological pathways for that specific drug.

### 6. Automated Testing (Pytest)
* Contains a robust suite of unit tests built with `pytest` (runnable directly in the Jupyter environment via `ipytest`).
* **Test Coverage:** Includes input validation, XML mock parsing, graph integrity checks (node/edge counts), and parametrized tests for the synthetic data generator.

## Technologies Used
* **Data Processing:** `Python`, `pandas`, `lxml`, `BeautifulSoup`
* **Network & Graphing:** `networkx`, `matplotlib`
* **Web API:** `fastapi`, `uvicorn`, `pydantic`, `requests`
* **Testing:** `pytest`, `ipytest`
