# Unity Display Info
Managed plugin for Unity to get detailed information about attached displays

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
| HiDpi Scale Factor          |          |        |         |        |          |    ✓   |         |    ✓   |
|                             |          |        |         |        |          |        |         |        |

\* <sub>For main display only</sub>

† <sub>Some macOS displays report a higher "Backing Store" resolution which is then scaled down to the screen resolution. This is what Unity natively reports.</sub>

### Virtual Display Resolution on macOS
If you use macOS, you might have noticed that the physical screen resolution is different from the value that Unity gives you. This is because macOS renders to a high resolution buffer called the Backing Store, which it then scales down to the native screen resolution. Here's an example using a MacBook Pro:

![mac_res](https://user-images.githubusercontent.com/793643/90556968-abf9be00-e191-11ea-85c6-89e7b3d03169.png)

Most of the time, everything will look fine and you won't have to worry about this. However, there are certain situations where the scaling/interpolation will make things look soft. For example, if you desperately wanted to fill the screen with black and white 1px lines. In this situation, you might want to change (or ask your user to change) the screen resolution to 1440 x 900 and change Unity's rendering resolution to match. Everything will then get doubled to 2880 x 1800 and will look nice and crisp.
