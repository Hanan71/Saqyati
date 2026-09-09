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
