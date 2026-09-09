let isRunning = false;

// إرسال نبضة التشغيل / الإيقاف إلى ESP8266
async function sendTriggerPulse() {
  const ip = document.getElementById('espIp').value.trim();
  const url = `http://${ip}/trigger`;

  try {
    const response = await fetch(url, {
      method: 'POST',
      mode: 'cors'
    });

    if (!response.ok) {
      throw new Error(`خطأ في الرد من الخادم: ${response.status}`);
    }

    const data = await response.json();
    isRunning = data.misting;
    updateUI();
  } catch (err) {
    console.error('تفاصيل الخطأ:', err);
    alert('تعذر الوصول إلى ESP8266!\nتأكد أنك متصل بنفس شبكة Hanan وأن الـ IP صحيح.');
  }
}

function updateUI() {
  const btn = document.getElementById('mistBtn');
  const text = document.getElementById('btnText');

  if (isRunning) {
    btn.classList.add('active');
    text.innerText = 'إيقاف الرذاذ';
  } else {
    btn.classList.remove('active');
    text.innerText = 'تشغيل الرذاذ';
  }
}

async function sendAgentMessage() {
  const input = document.getElementById('chatInput');
  const chatBox = document.getElementById('chatBox');
  const espIp = document.getElementById('espIp').value.trim();
  const text = input.value.trim();
  if (!text) return;

  // عرض رسالة المستخدم
  chatBox.innerHTML += `
    <div style="background: rgba(6, 182, 212, 0.2); padding: 8px 14px; border-radius: 10px; align-self: flex-end; max-width: 80%; font-size: 0.9rem;">
      ${text}
    </div>
  `;
  input.value = '';
  chatBox.scrollTop = chatBox.scrollHeight;

  // إظهار حالة جاري التفكير
  const loadingId = 'loading-' + Date.now();
  chatBox.innerHTML += `<div id="${loadingId}" style="color: var(--text-muted); font-size: 0.8rem;">جاري التفكير والتنفيذ...</div>`;
  chatBox.scrollTop = chatBox.scrollHeight;

  try {
    const res = await fetch('http://127.0.0.1:8000/api/chat', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ message: text, esp_ip: espIp })
    });
    const data = await res.json();
    document.getElementById(loadingId).remove();

    // عرض رد المساعد
    chatBox.innerHTML += `
      <div style="background: rgba(16, 185, 129, 0.15); padding: 8px 14px; border-radius: 10px; align-self: flex-start; max-width: 80%; font-size: 0.9rem;">
        ${data.reply}
      </div>
    `;

    // مزامنة حالة الزر والواجهة عند تنفيذ أمر الري بواسطة المساعد
    if (data.action_taken) {
      isRunning = !isRunning;
      updateUI();
    }
  } catch (err) {
    document.getElementById(loadingId).remove();
    chatBox.innerHTML += `
      <div style="background: rgba(239, 68, 68, 0.2); color: #fca5a5; padding: 8px 14px; border-radius: 10px; align-self: flex-start; max-width: 80%; font-size: 0.85rem;">
        تعذر الاتصال بـ Agent Server (تأكدي من تشغيل agent_server.py).
      </div>
    `;
  }
  chatBox.scrollTop = chatBox.scrollHeight;
}
