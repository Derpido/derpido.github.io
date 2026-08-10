// =========================================================
// HERO: typed boot sequence -> typed title -> cascading reveal
// =========================================================
const BOOT = [
  "> loading profile...",
  "> role: electrical_engineering_student",
  "> status: open_to_co-op_2027"
];

// title typed in segments so "sameen" keeps its accent colour
const TITLE = [
  { text: "hi, ",   accent: false },
  { text: "sameen", accent: true  },
  { text: " here.", accent: false }
];

const termEl  = document.getElementById("term");
const titleEl = document.getElementById("heroTitle");
const descEl  = document.getElementById("heroDesc");
const linksEl = document.getElementById("heroLinks");

const reduceMotion = window.matchMedia("(prefers-reduced-motion: reduce)").matches;

function makeCursor(cls){
  const c = document.createElement("span");
  c.className = cls;
  return c;
}

function revealIn(el, delay){
  if (!el) return;
  setTimeout(() => el.classList.add("is-in"), delay);
}

if (termEl && titleEl){
  if (reduceMotion){
    // no animation — show the final state, cursor still blinks
    termEl.textContent = BOOT.join("\n");
    titleEl.appendChild(makeCursor("title-cursor"));
    [descEl, linksEl].forEach(el => el && el.classList.add("is-in"));
  } else {
    // hide the parts that come after the title (JS-only, so no-JS still shows them)
    [descEl, linksEl].forEach(el => el && el.classList.add("pre-reveal"));

    const termCursor = makeCursor("cursor");
    titleEl.innerHTML = "";           // clear the static fallback title

    let li = 0, ci = 0;

    function typeBoot(){
      if (li >= BOOT.length){
        termCursor.remove();
        setTimeout(typeTitle, 260);   // brief beat, then the title
        return;
      }
      const line = BOOT[li];
      if (ci <= line.length){
        termEl.textContent =
          BOOT.slice(0, li).join("\n") + (li > 0 ? "\n" : "") + line.slice(0, ci);
        termEl.appendChild(termCursor);
        ci++;
        setTimeout(typeBoot, 16);
      } else {
        li++; ci = 0;
        setTimeout(typeBoot, 200);
      }
    }

    // ---- title ----
    const titleCursor = makeCursor("title-cursor");
    let si = 0, chi = 0;
    const spans = [];

    function typeTitle(){
      if (si >= TITLE.length){
        titleEl.appendChild(titleCursor);   // cursor stays, blinking
        revealIn(descEl, 180);
        revealIn(linksEl, 380);
        return;
      }
      const seg = TITLE[si];
      if (!spans[si]){
        const sp = document.createElement("span");
        if (seg.accent) sp.className = "accent";
        titleEl.appendChild(sp);
        spans[si] = sp;
      }
      if (chi <= seg.text.length){
        spans[si].textContent = seg.text.slice(0, chi);
        titleEl.appendChild(titleCursor);   // keep cursor trailing the text
        chi++;
        setTimeout(typeTitle, 55);
      } else {
        si++; chi = 0;
        setTimeout(typeTitle, 40);
      }
    }

    termEl.textContent = "";
    setTimeout(typeBoot, 350);
  }
}

// =========================================================
// Mobile nav toggle
// =========================================================
const navToggle = document.getElementById("navToggle");
const navLinks  = document.getElementById("navLinks");
if (navToggle && navLinks){
  navToggle.addEventListener("click", () => navLinks.classList.toggle("open"));
  navLinks.querySelectorAll("a").forEach(a =>
    a.addEventListener("click", () => navLinks.classList.remove("open"))
  );
}

// =========================================================
// Experience tabs
// =========================================================
const expTabs   = document.querySelectorAll(".exp-tab");
const expPanels = document.querySelectorAll(".exp-panel");
expTabs.forEach(tab => {
  tab.addEventListener("click", () => {
    expTabs.forEach(t => t.classList.remove("active"));
    expPanels.forEach(p => p.classList.remove("active"));
    tab.classList.add("active");
    const target = document.getElementById(tab.dataset.target);
    if (target) target.classList.add("active");
  });
});

// =========================================================
// Sections fade in as they scroll into view
// =========================================================
if (!reduceMotion && "IntersectionObserver" in window){
  const secs = document.querySelectorAll(".section");
  secs.forEach(s => s.classList.add("sec-hidden"));
  const io = new IntersectionObserver((entries) => {
    entries.forEach(e => {
      if (e.isIntersecting){
        e.target.classList.add("sec-in");
        io.unobserve(e.target);
      }
    });
  }, { threshold: 0.08, rootMargin: "0px 0px -40px 0px" });
  secs.forEach(s => io.observe(s));
}
