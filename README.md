# Unity Display Info
Managed plugin to get information about attached displays

## Feature Comparison
|                             |  Native  |        |         |        |  Plugin  |        |         |        |
|-----------------------------|:--------:|:------:|:-------:|:------:|:--------:|:------:|:-------:|:------:|
|                             | *Editor* |        | *Build* |        | *Editor* |        | *Build* |        |
|                             |   *Win*  |  *Mac* |  *Win*  |  *Mac* |   *Win*  |  *Mac* |  *Win*  |  *Mac* |
| Display Count               |     ✘    |    ✘   |    ✓    |    ✓   |          |    ✓   |         |    ✓   |
| Rendering Resolution        |    ✓*    |   ✓*   |    ✓    |    ✓   |          |    ✓   |         |    ✓   |
| Screen Resolution           |    ✓*    |   ✓*   |    ✓    |    ✓   |          |    ✓   |         |    ✓   |
| Native Display Resolution   |          |    ✘   |         |    ✘   |          |    ✓   |         |    ✓   |
| Virtual Display Resolution† |    N/A   |    ✘   |   N/A   |    ✘   |    N/A   |    ✓   |   N/A   |    ✓   |
| Physical Screen Size        |     ✘    |    ✘   |    ✘    |    ✘   |          |    ✓   |         |    ✓   |
| Display Bounds              |     ✘    |    ✘   |    ✘    |    ✘   |          |    ✓   |         |    ✓   |
| DPI                         |    ✓*    |   ✓*   |   ✓*    |   ✓*   |          |    ✓   |         |    ✓   |
| Refresh Rate                |          |        |         |        |          |        |         |        |
| HiDpi Scale Factor          |          |        |         |        |          |        |         |        |
|                             |          |        |         |        |          |        |         |        |

\* <sub>For main display only</sub>

† <sub>Some macOS displays report a higher "Backing Store" resolution which is then scaled down to the screen resolution. This is what Unity natively reports.</sub>

