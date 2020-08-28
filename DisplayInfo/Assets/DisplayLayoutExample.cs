using UnityEngine;
using UnityEngine.UI;
using highlyinteractive.diagnostics;

public class DisplayLayoutExample : MonoBehaviour
{
	public RectTransform Container;
	public RectTransform DisplaySpace;

	private AspectRatioFitter _fitter;

	private void Start ()
	{
		//Call this first to grab the current screen configuration
		DisplayInformation.RefreshDisplayData();

		//Get display count
		int count = DisplayInformation.DisplayCount;
		float min_w = 0, max_w = 0, min_h = 0, max_h = 0;

		//Work out display space
		for (int i = 0; i < count; i++)
		{
			Rect r = DisplayInformation.Bounds(i);

			min_w = Mathf.Min(min_w, r.x);
			min_h = Mathf.Min(min_h, r.y);

			max_w = Mathf.Max(max_w, r.x + r.width);
			max_h = Mathf.Max(max_h, r.y + r.height);
		}

		//Set display space dimensions
		float w = Mathf.Abs(min_w) + Mathf.Abs(max_w);
		float h = Mathf.Abs(min_h) + Mathf.Abs(max_h);
		DisplaySpace.sizeDelta = new Vector2(w, h);

		for (int i = 0; i < count; i++)
		{
			GameObject go = new GameObject("Display " + i);
			go.transform.SetParent(DisplaySpace.transform);
			Image img = go.AddComponent<Image>();
			img.color = new Color(1, 1, 1, 0.5f);

			Rect r = DisplayInformation.Bounds(i);
			float x = r.x + Mathf.Abs(min_w);
			float y = r.y + Mathf.Abs(min_h);
			img.rectTransform.anchorMin = new Vector2(0, 0);
			img.rectTransform.anchorMax = new Vector2(0, 0);
			img.rectTransform.pivot = new Vector2(0, 0);
			img.rectTransform.anchoredPosition = new Vector2(x, y);
			img.rectTransform.sizeDelta = new Vector2(r.width, r.height);
		}

		float sx = Container.rect.width / DisplaySpace.sizeDelta.x;
		float sy = Container.rect.height / DisplaySpace.sizeDelta.y;
		float s = Mathf.Min(sx, sy);
		DisplaySpace.localScale = new Vector3(s, -s, s);
	}
}
