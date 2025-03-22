import os
import glob
from hipporag import HippoRAG

# Prepare datasets and evaluation
markdown_dir = os.path.join(os.path.dirname(__file__), 'projectspecs')

# 读取所有 Markdown 文件
docs = []
markdown_files = glob.glob(os.path.join(markdown_dir, "*.md"))
for md_file in markdown_files:
    with open(md_file, 'r', encoding='utf-8') as f:
        content = f.read()
        docs.append(content)

save_dir = 'outputs'# Define save directory for HippoRAG objects (each LLM/Embedding model combination will create a new subdirectory)
llm_model_name = 'gpt-4o-mini' # Any OpenAI model name
embedding_model_name = 'nvidia/NV-Embed-v2'# Embedding model name (NV-Embed, GritLM or Contriever for now)

#Startup a HippoRAG instance
hipporag = HippoRAG(save_dir=save_dir, 
                    llm_model_name=llm_model_name,
                    embedding_model_name=embedding_model_name) 

#Run indexing
hipporag.index(docs=docs)

# TODO: Separate Retrieval & QA
queries = [
    "What is George Rankin's occupation?",
    "How did Cinderella reach her happy ending?",
    "What county is Erik Hort's birthplace a part of?"
]

retrieval_results = hipporag.retrieve(queries=queries, num_to_retrieve=2)
qa_results = hipporag.rag_qa(retrieval_results)

#Combined Retrieval & QA
rag_results = hipporag.rag_qa(queries=queries)

# TODO: For Evaluation
# answers = [
#     ["Politician"],
#     ["By going to the ball."],
#     ["Rockland County"]
# ]

# gold_docs = [
#     ["George Rankin is a politician."],
#     ["Cinderella attended the royal ball.",
#     "The prince used the lost glass slipper to search the kingdom.",
#     "When the slipper fit perfectly, Cinderella was reunited with the prince."],
#     ["Erik Hort's birthplace is Montebello.",
#     "Montebello is a part of Rockland County."]
# ]

# rag_results = hipporag.rag_qa(queries=queries, 
#                               gold_docs=gold_docs,
#                               gold_answers=answers)