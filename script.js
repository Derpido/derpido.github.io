// Terminal-style boot sequence in the hero section
const lines = [
  "> loading profile...",
  "> role: electrical_engineering_student",
  "> status: open_to_co-op_2027"
];

const el = document.getElementById('term');
const reduceMotion = window.matchMedia('(prefers-reduced-motion: reduce)').matches;

if (el) {
  if (reduceMotion) {
    el.textContent = lines.join('\n');
  } else {
    let li = 0, ci = 0;
    const cursor = document.createElement('span');
    cursor.className = 'cursor';

    function typeNext() {
      if (li >= lines.length) {
        el.appendChild(cursor);
        return;
      }
      const line = lines[li];
      if (ci <= line.length) {
        el.textContent = lines.slice(0, li).join('\n') + (li > 0 ? '\n' : '') + line.slice(0, ci);
        ci++;
        setTimeout(typeNext, 18);
      } else {
        li++; ci = 0;
        setTimeout(typeNext, 220);
      }
    }
    typeNext();
  }
}
