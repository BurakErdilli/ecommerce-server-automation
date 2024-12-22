import pandas as pd
import os
import logging

# Set up logging to log to both console and file
logger = logging.getLogger()
logger.setLevel(logging.INFO)

# File handler for logging to a file
file_handler = logging.FileHandler('kampanya_script_output.log')
file_handler.setLevel(logging.INFO)

# Stream handler for logging to terminal
stream_handler = logging.StreamHandler()
stream_handler.setLevel(logging.INFO)

# Formatter to format log messages
formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
file_handler.setFormatter(formatter)
stream_handler.setFormatter(formatter)

# Add handlers to logger
logger.addHandler(file_handler)
logger.addHandler(stream_handler)

# Directory paths
current_dir = os.path.dirname(os.path.abspath(__file__))
SALLES_DIR = os.path.join(current_dir, "indirilen_dosya_kampanya")
OUTPUT = "kampanya"
VIEWED = 10  # Max number of views for a product to be selected
MAX_PRICE_DIFFERENCE = 0.05

# Load the favorite viewing report Excel file
try:
    xlsx_files = [
        file
        for file in os.listdir(current_dir)
        if "favori-görüntüleme-raporu" in file and file.endswith(".xlsx")
    ]
    if not xlsx_files:
        raise FileNotFoundError(
            "No 'favori-görüntüleme-raporu' file found in current_dir.")

    df_favorite = pd.read_excel(os.path.join(current_dir, xlsx_files[0]))
    logger.info("File loaded successfully into df_favorite.")
except Exception as e:
    logger.error(f"Failed to load favorite viewing report: {str(e)}")
    raise

# Load the product file
try:
    products_files = [
        file
        for file in os.listdir(SALLES_DIR)
        if "indirilen_dosya_kampanya" not in file and file.endswith(".xlsx")
    ]
    if not products_files:
        raise FileNotFoundError(
            "No suitable product file found in SALLES_DIR.")

    df_products = pd.read_excel(os.path.join(SALLES_DIR, products_files[0]))
    logger.info("File loaded successfully into df_products.")
except Exception as e:
    logger.error(f"Failed to load product file: {str(e)}")
    raise

# Filter products with "Mevcut Stok" > 0 and "Mevcut Satış Fiyatı" > 0
df_products = df_products[
    (df_products["Mevcut Stok"] > 0) & (df_products["Mevcut Satış Fiyatı"] > 0)
]
logger.info(f"Filtered products based on stock and price, remaining products: {
            len(df_products)}")

# Filter products based on MAX_PRICE_DIFFERENCE
df_products = df_products[
    (df_products["Mevcut Satış Fiyatı"] -
     df_products["Maksimum Girebileceğin Fiyat"])
    / df_products["Mevcut Satış Fiyatı"]
    < MAX_PRICE_DIFFERENCE
]
logger.info(f"Filtered products based on price difference, remaining products: {
            len(df_products)}")

# Filter products based on Toplam Görüntülenme Sayısı from df_favorite
df_products = df_products[
    df_products["Ürün Adı"].isin(
        df_favorite[df_favorite["Toplam Görüntülenme Sayısı"]
                    < VIEWED]["Ürün Adı"]
    )
]
logger.info(f"Filtered products based on view count, remaining products: {
            len(df_products)}")

# Fill 'Kampanyalı Satış Fiyatı' with 'Maksimum Girebileceğin Fiyat'
df_products["Kampanyalı Satış Fiyatı"] = df_products["Maksimum Girebileceğin Fiyat"]

# Save the filtered products to a new Excel file
try:
    if not os.path.exists(OUTPUT):
        os.makedirs(OUTPUT)

    output_file_path = os.path.join(OUTPUT, "filtered_products.xlsx")
    df_products.to_excel(output_file_path, index=False)
    logger.info(f"Filtered products saved to {output_file_path}")
except Exception as e:
    logger.error(f"Failed to save filtered products: {str(e)}")
    raise

# Output the result
logger.info("Filtered and sorted products:")
logger.info(df_products.head())  # Output the first few rows for inspection
