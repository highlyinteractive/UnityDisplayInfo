using UnityEngine;
using UnityEngine.UI;
using highlyinteractive.diagnostics;

public class DisplayInfoExample : MonoBehaviour
{
	[Header("Desired Resolution")]
	public int Width = 1920;
	public int Height = 1080;

	[Header("")]
	public RectTransform Container;

	private void Start ()
	{
		//For demo purposes, set a rendering resolution
		PlayerPrefs.DeleteAll();
		Screen.SetResolution(Width, Height, false);

		CreateHeadings();
		CreateUnityData();
		CreatePluginData();
	}



	//* GET DISPLAY INFORMATION FROM PLUGIN ********************************* //

	private void CreatePluginData ()
	{
		//Call this first to grab the current screen configuration
		DisplayInformation.RefreshDisplayData();

		//Get display count
		int count = DisplayInformation.DisplayCount;

		for (int i = 0; i < count; i++)
		{
			string txt = $"<color=#00ff00><b>[Plugin]\nDisplay #{i}</b></color>\n";
			txt += $"{i} / {count}\n";
			if (DisplayInformation.RenderingWidth(i) == -1) txt += "Not rendering\n";
			else txt += $"{DisplayInformation.RenderingWidth(i)} x {DisplayInformation.RenderingHeight(i)}\n";
			txt += $"{DisplayInformation.ScreenWidth(i)} x {DisplayInformation.ScreenHeight(i)}\n";
			txt += $"{DisplayInformation.PixelWidth(i)} x {DisplayInformation.PixelHeight(i)}\n";
			txt += $"{DisplayInformation.VirtualWidth(i)} x {DisplayInformation.VirtualHeight(i)}\n";
			txt += $"{DisplayInformation.PhysicalWidth(i)} x {DisplayInformation.PhysicalHeight(i)}mm ";
			txt += $"({DisplayInformation.PhysicalDiagonal(i).ToString("F0")}\")\n";
			Rect r = DisplayInformation.Bounds(i);
			txt += $"x:{r.x}, y:{r.y}, w:{r.width}, h:{r.height}\n";
			txt += $"{DisplayInformation.Dpi(i)}\n";
			txt += $"{DisplayInformation.RefreshRate(i)}\n";
			txt += $"{DisplayInformation.ScaleFactor(i)}\n";

			CreateTextColumn("Plugin Values", txt);
		}
	}



	//* GET DISPLAY INFORMATION FROM NATIVE UNITY API *********************** //

	private void CreateUnityData ()
	{
		//Get display count (this always returns 1 in the Editor)
		int count = Display.displays.Length;

		for (int i = 0; i < count; i++)
		{


			string txt = $"<color=cyan><b>[Unity]\nDisplay #{i}</b></color>\n";
			txt += $"{i} / {count}\n";
			txt += GetUnityRenderingResolution(i);
			txt += GetUnityScreenResolution(i);
			txt += $"\n\n\n\n";
			txt += $"{Screen.dpi}\n";
			txt += $"{Screen.currentResolution.refreshRate}\n";

			CreateTextColumn("Unity Values", txt);
		}
	}

	private string GetUnityRenderingResolution (int id)
	{
		if (id < Display.displays.Length)
		{
			Display display = Display.displays[id];

			return $"{display.renderingWidth} x {display.renderingHeight}\n";
		}

		return $"{Screen.currentResolution.width} x {Screen.currentResolution.height}\n";
	}

	private string GetUnityScreenResolution (int id)
	{
		if (id < Display.displays.Length && id > 0)
		{
			Display display = Display.displays[id];

			return $"{display.systemWidth} x {display.systemHeight}\n";
		}

		return $"{Screen.currentResolution.width} x {Screen.currentResolution.height}\n";
	}



	//*********************************************************************** //

	private void CreateHeadings ()
	{
		string txt = "<b>\n\n" +
			"Display ID / Count: \n" +
			"Rendering Resolution: \n" +
			"Screen Resolution: \n" +
			"Native Screen Resolution: \n" +
			"Virtual Screen Resolution: \n" +
			"Physical Screen Size: \n" +
			"Display Bounds: \n" +
			"DPI: \n" +
			"Refresh Rate: \n" +
			"HiDPI Scale Factor: " +
			"</b>";

		CreateTextColumn("Text Headings", txt, false);
	}

	private void CreateTextColumn (string label, string content, bool c = true)
	{
		GameObject go = new GameObject(label);
		go.transform.SetParent(Container.transform);

		Text txt = go.AddComponent<Text>();
		txt.font = Resources.GetBuiltinResource<Font>("Arial.ttf");
		txt.verticalOverflow = VerticalWrapMode.Overflow;
		if (c) txt.alignment = TextAnchor.UpperCenter;
		txt.lineSpacing = 1.4f;
		txt.text = content;
	}
}
