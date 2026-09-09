import httpx
from fastapi import FastAPI, Request, Response
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
from google import genai
from google.genai import types

API_KEY = "AQ.Ab8RN6KUYXdS29SpEkb5yW9gm3sRzeyj6dijEqJQwjqGZsh_Zg"
client = genai.Client(api_key=API_KEY)

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

class ChatRequest(BaseModel):
    message: str
    esp_ip: str = "172.20.10.6"

@app.get("/")
def home():
    return {"status": "ok", "message": "Saqyati Agent Server is running"}

@app.post("/api/chat")
async def chat_with_agent(req: ChatRequest):
    action_taken = False
    action_msg = ""
    lower_msg = req.message.strip().lower()
    
    # فحص طلب تشغيل الرذاذ
    mist_keywords = ["اسق", "رش", "بخر", "شغل", "اطف", "وقف", "سقي", "ماء"]
    if any(k in lower_msg for k in mist_keywords):
        url = f"http://{req.esp_ip}/trigger"
        try:
            async with httpx.AsyncClient(timeout=3.0) as http_client:
                res = await http_client.post(url)
                data = res.json()
                state = "يعمل الآن" if data.get("misting") else "متوقف الآن"
                action_msg = f"تم إرسال الأمر للوحة، حالة الرذاذ: {state}"
                action_taken = True
        except Exception as e:
            action_msg = f"فشل الاتصال باللوحة ({url}): {str(e)}"
            action_taken = False

    prompt = req.message
    if action_taken:
        prompt = f"المستخدم طلب ري النبات قائلاً: '{req.message}'. تم إرسال نبضة الري بنجاح للوحة ({action_msg}). أكد للمستخدم بلطف واختصار أن الرذاذ يعمل الآن."

    try:
        response = client.models.generate_content(
            model="gemini-3.6-flash",
            contents=prompt,
            config=types.GenerateContentConfig(
                system_instruction="أنت مرشد ساقيتي الذكي، خبير رعاية نباتات ومساعد منزلي لنظام الزراعة الضبابية. أجب بلباقة ولغة عربية سليمة ومختصرة.",
                temperature=0.3
            )
        )
        reply_text = response.text
    except Exception as e:
        reply_text = f"حدث خطأ أثناء معالجة رد الذكاء الاصطناعي: {str(e)}"

    return {
        "reply": reply_text,
        "action_taken": action_taken
    }

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="127.0.0.1", port=8000)