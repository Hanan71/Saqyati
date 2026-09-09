#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>

const int triggerPin = D1; // GPIO5
ESP8266WebServer server(80);
bool isMisting = false;

// إرسال نبضة مؤكدة ونظيفة ثم إرجاع المنفذ فوراً لحالة مستقرة
void pressMistButton() {
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  delay(150); // نبضة الضغط
  digitalWrite(triggerPin, HIGH);
  pinMode(triggerPin, INPUT_PULLUP);
}

void setCORSHeaders() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "*");
}

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="ar" dir="rtl">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>لوحة تحكم ساقيتي | Saqyati</title>
  <link rel="preconnect" href="https://fonts.googleapis.com">
  <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
  <link href="https://fonts.googleapis.com/css2?family=Tajawal:wght@400;500;700;800&display=swap" rel="stylesheet">
  <style>
    :root {
      --bg: #0b1315;
      --card-bg: rgba(18, 30, 32, 0.85);
      --card-border: rgba(38, 166, 154, 0.25);
      --primary: #10b981;
      --accent-cyan: #06b6d4;
      --text: #e6f1f2;
      --text-muted: #94a3b8;
      --danger: #ef4444;
      --future: #8b5cf6;
    }
    * { box-sizing: border-box; margin: 0; padding: 0; font-family: 'Tajawal', sans-serif; }
    body {
      background: radial-gradient(circle at top right, #12282c, #070d0f);
      color: var(--text);
      min-height: 100vh;
      padding: 20px;
    }
    .container { max-width: 800px; margin: 0 auto; display: flex; flex-direction: column; gap: 20px; }
    header {
      background: var(--card-bg);
      border: 1px solid var(--card-border);
      padding: 16px 24px;
      border-radius: 16px;
      display: flex;
      justify-content: space-between;
      align-items: center;
      backdrop-filter: blur(8px);
    }
    .brand { display: flex; align-items: center; gap: 10px; }
    .brand h1 { font-size: 1.5rem; color: var(--primary); }
    .status-badge {
      display: inline-flex;
      align-items: center;
      gap: 8px;
      background: rgba(16, 185, 129, 0.1);
      border: 1px solid var(--primary);
      padding: 6px 14px;
      border-radius: 20px;
      font-size: 0.85rem;
      color: var(--primary);
    }
    .dot {
      width: 8px;
      height: 8px;
      background: var(--primary);
      border-radius: 50%;
      box-shadow: 0 0 8px var(--primary);
    }
    .card {
      background: var(--card-bg);
      border: 1px solid var(--card-border);
      border-radius: 16px;
      padding: 24px;
      backdrop-filter: blur(8px);
    }
    .card-title {
      font-size: 1.15rem;
      font-weight: 700;
      margin-bottom: 16px;
      display: flex;
      align-items: center;
      gap: 8px;
    }
    .trigger-container {
      display: flex;
      flex-direction: column;
      align-items: center;
      gap: 14px;
      text-align: center;
    }
    .pulse-button {
      width: 140px;
      height: 140px;
      border-radius: 50%;
      border: none;
      background: linear-gradient(135deg, var(--primary), var(--accent-cyan));
      color: #fff;
      font-size: 1.15rem;
      font-weight: 700;
      cursor: pointer;
      box-shadow: 0 0 25px rgba(16, 185, 129, 0.35);
      transition: all 0.2s ease;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      gap: 6px;
    }
    .pulse-button:active { transform: scale(0.95); }
    .pulse-button.active {
      background: linear-gradient(135deg, var(--danger), #f87171);
      box-shadow: 0 0 25px rgba(239, 68, 68, 0.5);
    }
    .schedule-grid {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
      gap: 16px;
    }
    .input-group { display: flex; flex-direction: column; gap: 6px; }
    label { font-size: 0.9rem; color: var(--text-muted); }
    select, input[type="range"] {
      background: rgba(0, 0, 0, 0.3);
      border: 1px solid var(--card-border);
      color: #fff;
      padding: 10px;
      border-radius: 8px;
      outline: none;
    }
    .toggle-wrapper {
      display: flex;
      align-items: center;
      justify-content: space-between;
      margin-bottom: 16px;
    }
    .switch { position: relative; width: 50px; height: 26px; }
    .switch input { display: none; }
    .slider {
      position: absolute;
      cursor: pointer;
      inset: 0;
      background: #334155;
      border-radius: 34px;
      transition: 0.3s;
    }
    .slider:before {
      position: absolute;
      content: "";
      height: 18px;
      width: 18px;
      left: 4px;
      bottom: 4px;
      background: white;
      border-radius: 50%;
      transition: 0.3s;
    }
    input:checked + .slider { background: var(--primary); }
    input:checked + .slider:before { transform: translateX(24px); }
    .future-grid {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(230px, 1fr));
      gap: 12px;
    }
    .future-card {
      background: rgba(139, 92, 246, 0.05);
      border: 1px dashed rgba(139, 92, 246, 0.35);
      padding: 14px;
      border-radius: 12px;
      display: flex;
      flex-direction: column;
      gap: 6px;
    }
    .future-tag {
      font-size: 0.75rem;
      background: rgba(139, 92, 246, 0.2);
      color: #c4b5fd;
      padding: 2px 8px;
      border-radius: 6px;
      width: max-content;
    }
    .future-card h4 { font-size: 0.95rem; color: #fff; }
    .future-card p { font-size: 0.8rem; color: var(--text-muted); line-height: 1.4; }
  </style>
</head>
<body>
  <div class="container">
    <header>
      <div class="brand">
        <h1>ساقيتي</h1>
        <span style="color: var(--text-muted); font-size: 0.9rem;">| نظام التحكم بالرذاذ</span>
      </div>
      <div class="status-badge">
        <div class="dot"></div>
        <span>متصل مباشرة بـ ESP8266</span>
      </div>
    </header>

    <section class="card">
      <div class="card-title">التحكم المباشر (محاكاة زر الرذاذ)</div>
      <div class="trigger-container">
        <button id="mistBtn" class="pulse-button" onclick="sendTriggerPulse()">
          <span style="font-size: 2rem;">💧</span>
          <span id="btnText">تشغيل الرذاذ</span>
        </button>
        <p id="statusDesc" style="font-size: 0.85rem; color: var(--text-muted);">
          الحالة: وضع الانتظار (إرسال نبضة 150ms على منفذ D1)
        </p>
      </div>
    </section>

    <section class="card">
      <div class="toggle-wrapper">
        <div class="card-title" style="margin-bottom:0;">الري الأوتوماتيكي (جدولة ذاتية)</div>
        <label class="switch">
          <input type="checkbox" id="autoScheduleToggle" checked>
          <span class="slider"></span>
        </label>
      </div>
      <div class="schedule-grid">
        <div class="input-group">
          <label>التكرار الزمني</label>
          <select id="intervalSelect">
            <option value="4">كل 4 ساعات</option>
            <option value="6" selected>كل 6 ساعات</option>
            <option value="12">كل 12 ساعة</option>
            <option value="24">كل 24 ساعة (يومياً)</option>
          </select>
        </div>
        <div class="input-group">
          <label>مدة التبخير: <span id="durationValue">10</span> ثوانٍ</label>
          <input type="range" id="durationRange" min="5" max="60" value="10" oninput="document.getElementById('durationValue').innerText = this.value">
        </div>
      </div>
    </section>

    <section class="card">
      <div class="card-title" style="color: #c4b5fd;">
        <span>🚀</span> الميزات القادمة (مستقبلاً)
      </div>
      <div class="future-grid">
        <div class="future-card">
          <span class="future-tag">Hardware</span>
          <h4>Soil Moisture Sensor</h4>
          <p>معرفة حالة ورطوبة التربة فعلياً لجدولة التبخير عند الجفاف فقط.</p>
        </div>
        <div class="future-card">
          <span class="future-tag">Automation</span>
          <h4>تعديل الكمية والوقت تلقائياً</h4>
          <p>تغيير فترات الضخ وكميات الرذاذ بالاعتماد المباشر على قراءات الحساسات.</p>
        </div>
        <div class="future-card">
          <span class="future-tag">Multiple-Plant</span>
          <h4>تخصيص ري كل نبتة</h4>
          <p>توزيع الصمامات والري المخصص لكل نوع نبتة بدلاً من الجداول الموحدة.</p>
        </div>
        <div class="future-card">
          <span class="future-tag">Hardware</span>
          <h4>Water Level Sensor</h4>
          <p>تنبيه مباشر على الواجهة عند انخفاض منسوب مياه الخزان وإيقاف المضخة حمايةً لها.</p>
        </div>
        <div class="future-card">
          <span class="future-tag">AI / ML</span>
          <h4>نماذج Machine Learning</h4>
          <p>بناء نموذج ذكي يتنبأ بحاجة النباتات مستنداً إلى البيانات الحقيقية المحصودة.</p>
        </div>
        <div class="future-card">
          <span class="future-tag">Smart Care</span>
          <h4>AI Recommendations</h4>
          <p>نصائح وتوصيات زراعية ذكية حول الإضاءة، حرارة الجو وأوقات التسميد.</p>
        </div>
        <div class="future-card">
          <span class="future-tag">Nutrients</span>
          <h4>كبسولات الأسمدة والمغذيات</h4>
          <p>نظام لإضافة المغذيات والأسمدة المناسبة لماء الري تلقائياً دون تدخل يدوي.</p>
        </div>
      </div>
    </section>
  </div>

  <script>
    async function sendTriggerPulse() {
      const btn = document.getElementById('mistBtn');
      const text = document.getElementById('btnText');
      const desc = document.getElementById('statusDesc');

      try {
        const res = await fetch('/trigger', { method: 'POST' });
        const data = await res.json();

        if (data.misting) {
          btn.classList.add('active');
          text.innerText = 'إيقاف الرذاذ';
          desc.innerText = 'الحالة: التبخير يعمل الآن';
        } else {
          btn.classList.remove('active');
          text.innerText = 'تشغيل الرذاذ';
          desc.innerText = 'الحالة: وضع الانتظار';
        }
      } catch (err) {
        alert('حدث خطأ في الاتصال باللوحة');
      }
    }
  </script>
</body>
</html>
)rawliteral";
  server.send(200, "text/html; charset=utf-8", html);
}

void handleTrigger() {
  pressMistButton();
  isMisting = !isMisting;

  setCORSHeaders();
  String response = "{\"status\":\"ok\",\"misting\":" + String(isMisting ? "true" : "false") + "}";
  server.send(200, "application/json", response);

  Serial.print("Action: Button Pressed | Mist State: ");
  Serial.println(isMisting ? "ON" : "OFF");
}

void setup() {
  Serial.begin(115200);

  // تثبيت المنفذ بحالة رفع فورية لمنع إرسال أي نبضة إرضائية عند الإقلاع
  pinMode(triggerPin, INPUT_PULLUP);

  WiFiManager wifiManager;

  // مسح بيانات الشبكة المخزنة لإجبار اللوحة على بث صفحة الإعداد فوراً
  wifiManager.resetSettings();

  Serial.println("\nChecking connection or launching Portal...");

  if (!wifiManager.autoConnect("Saqyati-Setup")) {
    Serial.println("Failed to connect, restarting ESP...");
    delay(3000);
    ESP.restart();
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("Open link: http://");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/trigger", HTTP_POST, handleTrigger);
  server.on("/trigger", HTTP_OPTIONS, []() {
    setCORSHeaders();
    server.send(204);
  });

  server.begin();
}

void loop() {
  server.handleClient();
}