import os
import glob
import json

def build_corpus_json(md_folder, output_json):

    md_files = glob.glob(os.path.join(md_folder, "*.md"))
    all_items = []
    md_files = glob.glob(os.path.join(md_folder, "*.md"))
    md_files.sort()

    for idx, md_file in enumerate(md_files):
        filename = os.path.splitext(os.path.basename(md_file))[0]

        with open(md_file, "r", encoding="utf-8") as f:
            content = f.read()

        item = {
            "title": filename,
            "text": content,
            "idx": idx
        }
        all_items.append(item)

    with open(output_json, "w", encoding="utf-8") as f:
        json.dump(all_items, f, ensure_ascii=False, indent=2)

    print(f"[*] Successfully wrote {len(all_items)} items to {output_json}.")

if __name__ == "__main__":
    script_dir = os.path.dirname(__file__)
    # 让 md_folder 指向脚本所在目录
    md_folder = script_dir
    output_json = "mydata_corpus.json"

    build_corpus_json(md_folder, output_json)
