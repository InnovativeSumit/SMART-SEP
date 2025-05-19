document.addEventListener('DOMContentLoaded', function () {
    // Initialize audio
    const audio = document.getElementById('background-audio');
    const audioToggle = document.getElementById('audio-toggle');
    const volumeControl = document.getElementById('volume-control');
    let isAudioPlaying = false;

    // Set initial volume
    audio.volume = parseFloat(volumeControl.value);

    // Allow audio to start on first user interaction (browser requirement)
    document.body.addEventListener('click', function initAudio() {
        if (!isAudioPlaying) {
            audio.play().then(() => {
                isAudioPlaying = true;
                audioToggle.innerHTML = '<i class="fas fa-volume-up"></i>';
            }).catch(e => console.log('Audio play failed:', e));
        }
        document.body.removeEventListener('click', initAudio);
    }, { once: true });

    // Audio toggle button
    audioToggle.addEventListener('click', function () {
        if (isAudioPlaying) {
            audio.pause();
            audioToggle.innerHTML = '<i class="fas fa-volume-mute"></i>';
        } else {
            audio.play().then(() => {
                audioToggle.innerHTML = '<i class="fas fa-volume-up"></i>';
            }).catch(e => console.log('Audio play failed:', e));
        }
        isAudioPlaying = !isAudioPlaying;
    });

    // Volume control
    volumeControl.addEventListener('input', function () {
        audio.volume = parseFloat(this.value);
        if (this.value == 0) {
            audioToggle.innerHTML = '<i class="fas fa-volume-mute"></i>';
        } else {
            audioToggle.innerHTML = '<i class="fas fa-volume-up"></i>';
        }
    });

    // Initialize particles.js
    particlesJS('particles-js', {
        "particles": {
            "number": { "value": 80, "density": { "enable": true, "value_area": 800 } },
            "color": { "value": "#4fc3f7" },
            "shape": {
                "type": "circle",
                "stroke": { "width": 0, "color": "#000000" },
                "polygon": { "nb_sides": 5 }
            },
            "opacity": {
                "value": 0.5,
                "random": true,
                "anim": { "enable": true, "speed": 1, "opacity_min": 0.1, "sync": false }
            },
            "size": {
                "value": 3,
                "random": true,
                "anim": { "enable": true, "speed": 2, "size_min": 0.1, "sync": false }
            },
            "line_linked": {
                "enable": true,
                "distance": 150,
                "color": "#4fc3f7",
                "opacity": 0.2,
                "width": 1
            },
            "move": {
                "enable": true,
                "speed": 1,
                "direction": "none",
                "random": true,
                "straight": false,
                "out_mode": "out",
                "bounce": false,
                "attract": { "enable": true, "rotateX": 600, "rotateY": 1200 }
            }
        },
        "interactivity": {
            "detect_on": "canvas",
            "events": {
                "onhover": { "enable": true, "mode": "grab" },
                "onclick": { "enable": true, "mode": "push" },
                "resize": true
            },
            "modes": {
                "grab": { "distance": 140, "line_linked": { "opacity": 0.5 } },
                "bubble": { "distance": 400, "size": 40, "duration": 2, "opacity": 8, "speed": 3 },
                "repulse": { "distance": 200, "duration": 0.4 },
                "push": { "particles_nb": 4 },
                "remove": { "particles_nb": 2 }
            }
        },
        "retina_detect": true
    });

    // Slider logic
    const sliderControls = document.querySelector(".slider-controls");
    const sliderTabs = sliderControls.querySelectorAll(".slider-tab");
    const sliderIndicator = sliderControls.querySelector(".slider-indicator");

    const updateIndicator = (tab, index) => {
        document.querySelector(".slider-tab.current")?.classList.remove("current");
        tab.classList.add("current");
        sliderIndicator.style.transform = `translateX(${tab.offsetLeft}px)`;
        sliderIndicator.style.width = `${tab.getBoundingClientRect().width}px`;

        const scrollLeft = sliderTabs[index].offsetLeft - sliderControls.offsetWidth / 2 + sliderTabs[index].offsetWidth / 2;
        sliderControls.scrollTo({ left: scrollLeft, behavior: "smooth" });
    }

    const swiper = new Swiper(".slider-container", {
        effect: "fade",
        speed: 1300,
        autoplay: {
            delay: 5000,
            disableOnInteraction: false
        },
        navigation: {
            prevEl: "#slide-prev",
            nextEl: "#slide-next",
        },
        on: {
            slideChange: () => {
                const currentTabIndex = [...sliderTabs].indexOf(sliderTabs[swiper.activeIndex]);
                updateIndicator(sliderTabs[swiper.activeIndex], currentTabIndex);

                const activeSlide = document.querySelector('.swiper-slide-active');
                activeSlide.style.animation = 'none';
                void activeSlide.offsetWidth;
                activeSlide.style.animation = 'pulsate 0.5s ease';
            },
            reachEnd: () => swiper.autoplay.stop()
        },
    });

    const style = document.createElement('style');
    style.textContent = `
        @keyframes pulsate {
            0% { transform: scale(1); }
            50% { transform: scale(1.01); }
            100% { transform: scale(1); }
        }
    `;
    document.head.appendChild(style);

    sliderTabs.forEach((tab, index) => {
        tab.addEventListener("click", () => {
            swiper.slideTo(index);
            updateIndicator(tab, index);

            const ripple = document.createElement('span');
            ripple.classList.add('ripple-effect');
            tab.appendChild(ripple);

            setTimeout(() => {
                ripple.remove();
            }, 1000);
        });
    });

    updateIndicator(sliderTabs[0], 0);

    window.addEventListener("resize", () => {
        updateIndicator(sliderTabs[swiper.activeIndex], swiper.activeIndex);
    });

    const sliderItems = document.querySelectorAll('.slider-item');
    sliderItems.forEach(item => {
        item.addEventListener('mousemove', (e) => {
            const x = e.clientX / window.innerWidth;
            const y = e.clientY / window.innerHeight;

            const content = item.querySelector('.slide-content');
            if (content) {
                content.style.transform = `translate(${x * 20 - 10}px, ${y * 20 - 10}px)`;
            }
        });

        item.addEventListener('mouseleave', () => {
            const content = item.querySelector('.slide-content');
            if (content) {
                content.style.transform = 'translate(0, 0)';
            }
        });
    });
});
