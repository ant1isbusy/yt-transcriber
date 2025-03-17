from transformers import pipeline
import torch
import re

device = 0 if torch.cuda.is_available() else -1

summarizer = pipeline(
    "summarization", model="sshleifer/distilbart-cnn-12-6", device=device
)

with open("transcript.txt", "r") as file:
    transcript = file.read()

def chunk_text_by_words(text, max_words=500):
    words = text.split()
    chunks = [
        " ".join(words[i : i + max_words]) for i in range(0, len(words), max_words)
    ]
    return [chunk.strip() for chunk in chunks]


# regex from ChatGPT:
def clean_text(text):
    text = re.sub(r"\s+([.,!?])", r"\1", text)
    text = re.sub(r"([.,!?])(\S)", r"\1 \2", text)
    return text.strip()

chunks = chunk_text_by_words(transcript)

summary = "\n".join(
    clean_text(
        summarizer(
            chunk,
            max_length=200,
            min_length=50,
            do_sample=False
        )[0]["summary_text"]
    )
    for chunk in chunks
)

final_summary = clean_text(summary)

with open("summary.txt", "w") as file:
    file.write(final_summary)

print(final_summary)
