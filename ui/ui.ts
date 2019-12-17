/*
 * This file is part of the espx-dancing-led distribution (https://github.com/kluevandrew/espx-dancing-led).
 * Copyright (c) 2019, Kluev Andrew <kluev.andrew@gmail.com>.
 *
 * espx-dancing-led is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
 *
 * You are free to:
 *   Share — copy and redistribute the material in any medium or format
 *   Adapt — remix, transform, and build upon the material
 *
 * Under the following terms:
 *   Attribution — You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
 *   NonCommercial — You may not use the material for commercial purposes.
 *   ShareAlike — If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
 *   No additional restrictions — You may not apply legal terms or technological measures that legally restrict others from doing anything the license permits.
 *
 * For additional information, see <http://creativecommons.org/licenses/by-nc-sa/4.0/>.
 */
import { Api } from "./api";
import { Renderer } from "./renderer";
import { ANIMATIONS } from "./config";
import * as noUiSlider from "nouislider";
import ReinventedColorWheel from "reinvented-color-wheel";
import { SimpleGraph } from "./graph";


export class Ui {
  constructor(private api: Api, private renderer: Renderer) {
  }

  public init() {
    document.body.append(this.bind(this.renderer.render('app.html', {ANIMATIONS})));
    this.api.on('ota_start', () => {
      let text = 'Идёт обновление';
      this.showLoader("progress", { text });
    });
    this.api.on('ota_progress', (message) => {
      let text = 'Идёт обновление';
      let percent = message.progress / (message.total / 100);
      if (percent < 95) {
        this.showLoader("progress", { percent, text });
      } else {
        this.showLoader("infinity", { percent, text });
        setTimeout(() => {
          document.location.reload();
        }, 10000)
      }
    });
    this.api.onClose(() => {
      let text = 'Переподключение';
      this.showLoader("infinity", { text });
    });
    this.api.onConnect(() => {
      this.hideLoader();
    });
  }

  public bind(element: HTMLElement) {
    try {
      element.querySelectorAll('[data-animation-selector]').forEach((element: HTMLSelectElement) => {
        element.addEventListener('change', () => {
          this.setAnimation(element.value);
        })
      });

      element.querySelectorAll('[data-option-input]').forEach((optionInput: HTMLSelectElement) => {
        optionInput.addEventListener('change', () => {
          const value = parseInt(optionInput.value);
          this.api.setOption(optionInput.dataset.optionInput, value);
        });

        optionInput.value = this.api.getOption(optionInput.dataset.optionInput)
      });

      element.querySelectorAll('[data-save-settings]').forEach((element: HTMLSelectElement) => {
        element.addEventListener('click', () => {
          this.api.saveOptions().then(() => {
            alert("Настройки сохранены");
          });
        })
      });

      element.querySelectorAll('[data-reset-settings]').forEach((element: HTMLSelectElement) => {
        element.addEventListener('click', () => {
          if (confirm("Сбросить настройки?")) {
            this.api.resetOptions().then(() => {
              alert("Настройки сброшены");
            });
          }
        })
      });

      element.querySelectorAll('[data-slider]').forEach((slider: HTMLSelectElement) => {
        let value = this.api.getOption(slider.dataset.slider) || 0;

        let min;
        let max;
        let digits = parseInt(slider.dataset.float, 10);
        if (slider.dataset.float) {
          min = this.roundTo(parseFloat(slider.dataset.min), digits);
          max = this.roundTo(parseFloat(slider.dataset.max), digits);
        } else {
          min = parseInt(slider.dataset.min, 10);
          max = parseInt(slider.dataset.max, 10);
        }

        noUiSlider.create(slider, {
          start: value,
          // step: slider.dataset.step ? parseFloat(slider.dataset.step) : 1,
          range: {
            'min': min,
            'max': max,
          },
          pips: {
            mode: 'steps',
            stepped: true,
            density: 5
          },
          tooltips: true,
          format: {
            from: (value) => {
              return slider.dataset.float ? this.roundTo(parseFloat(value), digits) : parseInt(value);
            },
            to: (value) => {
              return slider.dataset.float ? this.roundTo(parseFloat(value), digits) : parseInt(value);
            }
          },
        }).on('change.one', (values) => {
          let value = values[0];
          this.api.setOption(slider.dataset.slider, value);
        });
      });

      element.querySelectorAll('[data-color]').forEach((picker: HTMLSelectElement) => {
        const colorWheel = new ReinventedColorWheel({
          appendTo: picker,
          hsv: [
            Ui.map(this.api.getOption(`${picker.dataset.color}.hue`), 0, 255, 0, 360),
            Ui.map(this.api.getOption(`${picker.dataset.color}.sat`), 0, 255, 0, 100),
            Ui.map(this.api.getOption(`${picker.dataset.color}.bright`), 0, 255, 0, 100),
          ],
          wheelDiameter: 200,
          wheelThickness: 20,
          handleDiameter: 16,
          wheelReflectsSaturation: false,
          onChange: this.debounce((color) => {
            this.api.setColorOption(picker.dataset.color, color.hsv[0], color.hsv[1], color.hsv[2]);
          }, 200).bind(this),
        });
        setImmediate(colorWheel.redraw.bind(colorWheel), 0)
      });

      element.querySelectorAll('[data-toggle-addition-options]').forEach((button: HTMLButtonElement) => {
        button.addEventListener('click', () => {
          element.querySelectorAll('[data-toggle-addition-options]').forEach((e: HTMLButtonElement) => {
            e.classList.remove('btn--primary');
            e.classList.add('btn--default');
          });
          button.classList.add('btn--primary');
          button.classList.remove('btn--default');

          this.loadAdditionOptions(button.dataset.toggleAdditionOptions);
        })
      });

      element.querySelectorAll('[data-refresh-wifi]').forEach((button: HTMLButtonElement) => {
        button.addEventListener('click', () => {
          this.loadAdditionOptions('wifi');
        })
      });

      element.querySelectorAll('[data-connect-wifi]').forEach((button: HTMLButtonElement) => {
        button.addEventListener('click', () => {
          const ssid = button.dataset.connectWifi;
          const secure = button.dataset.connectWifiSecure == '1';
          this.openModal('modals/wifi-connect.html', {ssid, secure}).then((form: HTMLFormElement) => {
            const input = form.querySelector<HTMLInputElement>('input[type=password]');
            let password = null;
            if (input) {
              password = input.value;
            }
            this.api.connectWifi(ssid, password)
          })
        })
      });

      element.querySelectorAll('[data-reset-wifi]').forEach((button: HTMLButtonElement) => {
        button.addEventListener('click', () => {
          if (confirm('Вы уверены что хотите сбросить настройки WiFi?')) {
            this.api.resetWifi();
          }
        })
      });
    } catch (e) {
      console.error(e)
    }

    return element;
  }

  public setAnimation(animation: string) {
    this.api.setAnimation(animation).then(() => {
      this.displayAnimationBlock(animation);
    })
  }

  private debounce(func, delay) {
    let debounceTimer;
    return (function () {
      const context = this;
      const args = arguments;
      clearTimeout(debounceTimer);
      debounceTimer = setTimeout(() => func.apply(context, args), delay)
    });
  }

  private static map(x: number, inMin: number, inMax: number, outMin: number, outMax: number): number {
    const divisor = (inMax - inMin);
    if (divisor == 0) {
      return -1; //AVR returns -1, SAM returns 0
    }
    return (x - inMin) * (outMax - outMin) / divisor + outMin;
  }

  private roundTo(n, digits) {
    if (digits === undefined) {
      digits = 0;
    }

    const multiplicator = Math.pow(10, digits);
    n = parseFloat((n * multiplicator).toFixed(11));
    const test = (Math.round(n) / multiplicator);
    return +(test.toFixed(digits));
  }

  public loadAdditionOptions(type: string) {
    let container = document.querySelector('[data-additional-options-content]');
    switch (type) {
      case 'wifi':
        this.api.getWifiInfo().then((response) => {
          container.innerHTML = '';
          container.append(
            this.bind(
              this.renderer.render('options/wifi.html', response),
            )
          )
        });
        break;
      case 'adc':
        container.innerHTML = '';
        container.append(
          this.bind(
            this.renderer.render('options/adc.html', {}),
          )
        );
        setImmediate(() => {
          const canvas = document.getElementById('adc-canvas') as HTMLCanvasElement;
          const graph = new SimpleGraph(canvas);
          const listener = function (payload: any) {
            try {
              graph.add(payload.lastMajorPeak);
            } catch (e) {
              console.error(e);
            }
          };
          this.api.on('frequencyAnalyze', listener);
          canvas.addEventListener('DOMNodeRemoved', () => {
            this.api.off("frequencyAnalyze", listener)
          });
        });
        break;
      case 'misc':
        container.innerHTML = '';
        container.append(
          this.bind(
            this.renderer.render('options/misc.html', {}),
          )
        );
        break;
    }
  }

  public hideLoader() {
    const loader = document.querySelector<HTMLDivElement>('[data-loader]');
    loader.classList.remove('loader--visible');
    loader.classList.remove('loader--infinity');
    loader.classList.remove('loader--progress');
  }

  public showLoader(type: string, options: any = {}) {
    const loader = document.querySelector<HTMLDivElement>('[data-loader]');
    if (options.percent) {
      loader.querySelector<HTMLDivElement>('[data-loader-progress]').style.width = `${options.percent}%`;
    } else {
      loader.querySelector<HTMLDivElement>('[data-loader-progress]').style.width = `0%`;
    }

    if (options.text) {
      loader.querySelector<HTMLDivElement>('[data-loader-text]').innerHTML = options.text;
    } else {
      loader.querySelector<HTMLDivElement>('[data-loader-text]').innerHTML = '';
    }

    loader.classList.add('loader--visible');
    loader.classList.remove('loader--infinity');
    loader.classList.remove('loader--progress');
    loader.classList.add(`loader--${type}`);
  }

  public showConnectionError() {
  }

  public hideConnectionError() {
  }

  public displayAnimationBlock(animation: string) {
    const settingsBlock = document.querySelector<HTMLDivElement>('[data-animation-settings]');
    settingsBlock.innerHTML = '';
    settingsBlock.append(this.bind(this.renderer.render(`animations/${animation}.html`, {animation: animation})));
    document.querySelectorAll('[data-animation-selector]').forEach((element: HTMLSelectElement) => {
      element.value = animation;
    });
  }

  private openModal(template: string, context: any) {
    return new Promise((resolve, reject) => {
      const overlay = document.querySelector<HTMLDivElement>('[data-modal-overlay]');
      const element = this.renderer.render(template, context);

      const bodyListener = function () {
        destroy();
        reject();
      };
      const destroy = () => {
        overlay.removeChild(element);
        overlay.innerHTML = '';
        overlay.classList.remove('modal-overlay--visible');
        document.body.removeEventListener('click', bodyListener);
      };
      element.querySelectorAll('[data-modal-close]').forEach((btn: HTMLButtonElement)  =>{
        btn.addEventListener('click', () => {
          destroy();
          reject();
        })
      });
      element.querySelectorAll('[data-modal-send]').forEach((btn: HTMLButtonElement)  =>{
        btn.addEventListener('click', () => {
          destroy();
          resolve(element.firstChild);
        })
      });
      element.addEventListener('click', (event) => {
        event.stopPropagation && event.stopPropagation();
        event.cancelBubble = true;
      });
      overlay.appendChild(element);
      overlay.classList.add('modal-overlay--visible');
      setImmediate(() => {
        document.body.addEventListener('click', bodyListener);
      })
    });
  }
}