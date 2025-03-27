import os
import glob
import re
import json
import multiprocessing

def main():
    # 在 main 内部导入 HippoRAG，避免在模块顶层启动多进程引发冲突
    from hipporag import HippoRAG

    # 假设当前脚本与 "projectspecs"、"discussionthreads" 文件夹同级
    markdown_dir = os.path.join(os.path.dirname(__file__), '..', 'projectspecs')
    query_dir = os.path.join(os.path.dirname(__file__), '..', 'discussionthreads')

    # 1) 读取 docs (来自 .md 文件)
    docs = []
    markdown_files = glob.glob(os.path.join(markdown_dir, "*.md"))
    for md_file in markdown_files:
        with open(md_file, 'r', encoding='utf-8') as f:
            content = f.read()
            docs.append(content)

    print("=== Docs loaded from 'projectspecs' ===")
    for i, doc in enumerate(docs):
        snippet = doc[:100].replace("\n", "\\n")  # 仅打印前100字符
        print(f"Doc[{i}] {snippet}...")

    # 2) 初始化 HippoRAG
    save_dir = 'outputs'
    llm_model_name = 'gpt-4o-mini'       # 若要连OpenAI实际模型，可改 'gpt-3.5-turbo' 等
    embedding_model_name = 'facebook/contriever'

    hipporag = HippoRAG(
        save_dir=save_dir, 
        llm_model_name=llm_model_name,
        embedding_model_name=embedding_model_name
    )

    # 3) 对 docs 进行索引
    hipporag.index(docs=docs)

    # 4) 读取 queries（假设文件名为 sp24_parsed.json）
    queries = []
    json_query_file = os.path.join(query_dir, "sp24_parsed.json")
    if os.path.isfile(json_query_file):
        with open(json_query_file, "r", encoding="utf-8") as f:
            data = json.load(f)  # data 应该是一个列表，每个元素带 "question" 字段
            for item in data:
                q = item.get("question", "").strip()
                if q:
                    queries.append(q)
    else:
        print(f"[WARNING] JSON file not found: {json_query_file}")

    print("=== Queries loaded from 'sp24_parsed.json' ===")
    for i, q in enumerate(queries):
        snippet = q.replace("\n", "\\n")
        print(f"Q[{i}]: {snippet}")

    # 5) 一步到位 RAG QA
    #    (HippoRAG 会内部执行检索 + 生成答案)
    rag_results = hipporag.rag_qa(queries=queries)
    print("=== RAG QA (One step) ===")

    for ans in rag_results:
        print(f"Question: {ans.question}")
        print(f"Answer:   {ans.answer}")
        print("----")

    clean_list = []
    for sol in rag_results:
        # sol 是一个 QuerySolution 实例
        # question 放在 sol.question
        # answer 放在 sol.answer
        # docs 里是大段检索上下文

        entry = {
            "question": sol.question,
            "answer": sol.answer  # 只要简洁答案, 不要整片 docs
        }
        clean_list.append(entry)

    # 写入 JSON
    with open("qa_results.json", "w", encoding="utf-8") as f:
        json.dump(clean_list, f, ensure_ascii=False, indent=2)

    print("[*] Done, see qa_results.json")

if __name__ == "__main__":
    # macOS 下把多进程启动方式改为 'fork'，避免 spawn 冲突
    multiprocessing.set_start_method("fork", force=True)

    main()
