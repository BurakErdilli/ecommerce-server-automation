import openpyxl
import os
import warnings
from openpyxl import Workbook

# Suppress UserWarning for missing default style
warnings.filterwarnings("ignore", category=UserWarning, module="openpyxl")

# Define the output directory
OUTPUT_DIR = 'kuponlar'
if not os.path.exists(OUTPUT_DIR):
    os.makedirs(OUTPUT_DIR)

# Price range
MAX_PRICE = 300
MIN_PRICE = 0

# Scan for the input Excel file
XLSX_FILE = next(
    (file for file in os.listdir("./") if file.startswith("Urunler") and file.endswith('.xlsx')),
    None
)

if not XLSX_FILE:
    print("No 'Urunler.xlsx' file found in the directory.")
else:
    # Load the Excel workbook
    xlsx_path = os.path.join("./", XLSX_FILE)
    workbook = openpyxl.load_workbook(xlsx_path)
    sheet = workbook.active

    # Get the headers
    headers = {cell.value: idx for idx, cell in enumerate(sheet[1], start=1)}

    # Ensure required columns exist
    if 'Güncel Satış Fiyatı' not in headers or 'Kupona Dahil Edilsin mi?' not in headers:
        print(f"Missing required headers in file: {XLSX_FILE}")
    else:
        rows_to_delete = []  # Track rows to delete

        # Process rows
        for row in sheet.iter_rows(min_row=2):
            try:
                # Clean and parse the price, handle both '259 ₺' and '285,62 ₺' formats
                price_str = str(row[headers['Güncel Satış Fiyatı'] - 1].value)
                price_cleaned = price_str.replace('₺', '').replace(',', '.').strip()
                price = int(float(price_cleaned))  # Convert to float first, then to integer

                # Update 'Kupona Dahil Edilsin mi?' column if within range
                if MIN_PRICE <= price <= MAX_PRICE:
                    row[headers['Kupona Dahil Edilsin mi?'] - 1].value = "Evet"
                else:
                    row[headers['Kupona Dahil Edilsin mi?'] - 1].value = "Hayır"

                # Collect rows to delete later
                if str(row[headers['Kupona Dahil Edilsin mi?'] - 1].value) == 'Hayır':
                    rows_to_delete.append(row[0].row)

            except Exception as e:
                print(f"Error at row {row[0].row}: {str(e)}")

        # Delete rows marked for deletion (in reverse to prevent index shifting)
        for row_idx in reversed(rows_to_delete):
            sheet.delete_rows(row_idx)

        # Save the output file
        output_file_path = os.path.join(OUTPUT_DIR, f"output_{XLSX_FILE}")
        workbook.save(output_file_path)
        print(f"Output saved to {output_file_path}")
