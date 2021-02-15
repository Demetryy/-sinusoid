WAVE-форма категории — PCM имеет следующий вид:

<WAVE-форма> = 'WAVE' + <fmt-чанк> + <data-чанк> , где 'WAVE' — сигнатура WAVE-формы
<fmt-чанк> — чанк с информацией о звуковом сигнале
<data-чанк> — чанк с собственно сигналом

<fmt-чанк> = 'fmt ' + <ckSize> + <WaveFormat> + <fmt-specific> , где
'fmt ' — сигнатура fmt-чанка
<ckSize> — его размер
<WaveFormat> — структура WaveFormat(mmsystem.h), описанная ниже
<fmt-specific> — структура с дополнительной информацией о формате, имеет переменную длину и зависит от wFormatCategory.
 
<data-чанк> = 'data' + <ckSize> + <собственно сигнал> , где 'data' — сигнатура data-чанка
<ckSize> — его размер
<собственно сигнал> — последовательность байт, описывающая сигнал (см. Формат данных PCM)
WaveFormat
Структура WaveFormat имеет вид:
  
typedef struct

{

WORD wFormatTag; // Категория формата

WORD nChannels; // Число каналов

DWORD nSamplesPerSec; // Частота дискретизации

DWORD nAvgBytesPerSec; // Байт в секунду

WORD nBlockAlign; // Выравнивание данных в data-чанке

} WaveFormat;

wFormatTag Категория формата (неудачный перевод: калька format category). От этого значения зависят значения остальных полей этой структуры, структура <fmt-specific> и data-чанка. Существует несколько категорий формата; самая доступная — PCM (Pulse CodeModulation) имеет wFormatTag = 1.

nChannels 1 — моно, 2-стерео, о большем числе каналов документация умалчивает.

nSamplesPerSec Частота дискретизации (число сэмплов в секунду).

nAvgBytesPerSec Среднее число байт в секунду, используется для эффективной буферизации. Для PCM вычисляется по формуле: (nChannels*nSamplesPerSec*nBitsPerSample)/8.

nBlockAlign Выравнивание данных в data-чанке. Для PCM вычисляется по формуле: (nChannels*nBitsPerSample)/8.


ВЗЯТО С САЙТА http://soobcha.lionovsky.us/faq/index.html@question=3434
