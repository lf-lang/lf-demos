#!/usr/bin/env python3

import csv
import sys
from pathlib import Path

SENSORS = {
    "front": "front_m",
    "bottom": "bottom_m",
    "left": "left_m",
    "right": "right_m",
    "top": "top_m",
}


def main() -> int:
    if len(sys.argv) != 3:
        print("Usage: python3 split_tof_csv.py input.csv output_dir")
        return 1

    input_path = Path(sys.argv[1])
    output_dir = Path(sys.argv[2])

    if not input_path.exists():
        print(f"Input file not found: {input_path}")
        return 1

    output_dir.mkdir(parents=True, exist_ok=True)

    with input_path.open("r", newline="") as infile:
        reader = csv.DictReader(infile)

        missing = [col for col in SENSORS.values() if col not in (reader.fieldnames or [])]
        if missing:
          print(f"Missing expected columns: {missing}")
          return 1

        files = {}
        writers = {}

        try:
            for sensor in SENSORS:
                fp = (output_dir / f"{sensor}.csv").open("w", newline="")
                files[sensor] = fp
                writer = csv.writer(fp)
                writers[sensor] = writer
                writer.writerow(["value_m"])

            for row in reader:
                for sensor, column in SENSORS.items():
                    value = row.get(column, "")
                    if value is None:
                        value = ""
                    writers[sensor].writerow([value])

        finally:
            for fp in files.values():
                fp.close()

    print(f"Wrote sensor CSVs to: {output_dir}")
    for sensor in SENSORS:
        print(output_dir / f"{sensor}.csv")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())