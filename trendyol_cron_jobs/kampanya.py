import pandas as pd
import os


current_dir = os.path.dirname(os.path.abspath(__file__))
# Directory paths
# Subdirectory for /indirilen_dosya_kampanya
SALLES_DIR = os.path.join(current_dir, "indirilen_dosya_kampanya")
OUTPUT = "kampanya"

VIEWED = 10  # MAX number of views for one product for being selected
MAX_PRICE_DIFFERENCE = 0.05


# Load the favorite viewing report Excel file
xlsx_files = [
    file
    for file in os.listdir(current_dir)
    if "favori-görüntüleme-raporu" in file and file.endswith(".xlsx")
]
if not xlsx_files:
    raise FileNotFoundError(
        "No 'favori-görüntüleme-raporu' file found in current_dir.")
df_favorite = pd.read_excel(os.path.join(current_dir, xlsx_files[0]))
print("File loaded successfully into df_favorite.")

# Load the product file
products_files = [
    file
    for file in os.listdir(SALLES_DIR)
    if "indirilen_dosya_kampanya" not in file and file.endswith(".xlsx")
]
if not products_files:
    raise FileNotFoundError("No suitable product file found in SALLES_DIR.")
df_products = pd.read_excel(os.path.join(SALLES_DIR, products_files[0]))
print("File loaded successfully into df_products.")

# Filter products with "Mevcut Stok" > 0 and "Mevcut Satış Fiyatı" > 0
df_products = df_products[
    (df_products["Mevcut Stok"] > 0) & (df_products["Mevcut Satış Fiyatı"] > 0)
]

# Filter products with ( "Mevcut Satış Fiyatı" -"Maksimum Girebileceğin Fiyat") / "Mevcut Satış Fiyatı"  MAX_PRICE_DIFFERENCE
df_products = df_products[
    (df_products["Mevcut Satış Fiyatı"] -
     df_products["Maksimum Girebileceğin Fiyat"])
    / df_products["Mevcut Satış Fiyatı"]
    < MAX_PRICE_DIFFERENCE
]

# # sort df_products by "Aktif Favori Sayısı"column they correspond in df_favorite by matching "Ürün Adı" of the two DataFrames
# df_products = df_products.sort_values(
#     by="Ürün Adı", key=lambda x: x.map(df_favorite.set_index('Ürün Adı')['Aktif Favori Sayısı']), ascending=False)

# filter df_products by "Toplam Görüntülenme Sayısı" < VIEWED column in df_favorites they correspond in df_favorite by matching "Ürün Adı" of the two DataFrames
df_products = df_products[
    df_products["Ürün Adı"].isin(
        df_favorite[df_favorite["Toplam Görüntülenme Sayısı"]
                    < VIEWED]["Ürün Adı"]
    )
]

# fill Kampanyalı Satış Fiyatı column with "Maksimum Girebileceğin Fiyat" fiyat column
df_products["Kampanyalı Satış Fiyatı"] = df_products["Maksimum Girebileceğin Fiyat"]

# save df_products to a new Excel file inside OUTPUT folder if it doesn't exist make it

if not os.path.exists(OUTPUT):
    os.makedirs(OUTPUT)

df_products.to_excel(os.path.join(
    OUTPUT, "filtered_products.xlsx"), index=False)

# Output the result
print("Filtered and sorted products:")
print(df_products)
