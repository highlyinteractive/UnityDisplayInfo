using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

namespace highlyinteractive.diagnostics
{
	public class DisplayInformation : MonoBehaviour
	{
		[DllImport("DisplayInfo", CallingConvention = CallingConvention.Cdecl)]
		private static extern int GetDisplayCount ();

		[DllImport("DisplayInfo", CallingConvention = CallingConvention.Cdecl)]
		private static extern void GetDisplayInformation (int[] array, int screenId = 0);

		private const int INFO_SIZE = 12;

		private static List<DisplayData> _displays = new List<DisplayData>();


		public static void RefreshDisplayData ()
		{
			int count = DisplayCount;

			for (int i = 0; i < count; i++)
			{
				int[] arr = new int[INFO_SIZE];

				GetDisplayInformation(arr, i);

				_displays.Add(new DisplayData(arr));
			}
		}

		#region Getters

		public static int DisplayCount => GetDisplayCount();

		//The rendering resolution (taken from Unity)
		public static int RenderingWidth (int id) => GetRenderingWidth(id);
		public static int RenderingHeight (int id) => GetRenderingHeight(id);

		//The current screen resolution (on macOS, this is the Background Store resolution)
		public static int ScreenWidth (int id) => id < _displays.Count ? _displays[id].ScreenWidth : -1;
		public static int ScreenHeight (int id) => id < _displays.Count ? _displays[id].ScreenHeight : -1;

		//The native resolution of the screen (physical pixels)
		public static int PixelWidth (int id) => id < _displays.Count ? _displays[id].PixelWidth : -1;
		public static int PixelHeight (int id) => id < _displays.Count ? _displays[id].PixelHeight : -1;

		//The virtual ("Looks Like") resolution reported by macOS (-1 on PC)
		public static int VirtualWidth (int id) => id < _displays.Count ? _displays[id].VirtualWidth : -1;
		public static int VirtualHeight (int id) => id < _displays.Count ? _displays[id].VirtualHeight : -1;

		//The physical display size
		public static float PhysicalWidth (int id) => id < _displays.Count ? _displays[id].PhysicalWidth : -1;
		public static float PhysicalHeight (int id) => id < _displays.Count ? _displays[id].PhysicalHeight : -1;
		public static float PhysicalDiagonal (int id) => GetDisplayDiagonal(id);

		//The display bounds (shows how monitors are virtually arranged)
		public static Rect Bounds (int id) => id < _displays.Count ? _displays[id].Bounds : new Rect();

		//The DPI based on the physical screen size
		public static float Dpi (int id) => GetDisplayDPI(id);

		//The scaling factor
		public static float ScaleFactor (int id) => GetScalingFactor(id);

		#endregion



		#region Helper Functions

		private static int GetRenderingWidth (int id)
		{
			if (id < Display.displays.Length)
			{
				return Display.displays[id].renderingWidth;
			}
			if (id == 0) return Screen.currentResolution.width;

			//TODO: Search all cameras in scene & check which display

			return -1;
		}

		private static int GetRenderingHeight (int id)
		{
			if (id < Display.displays.Length)
			{
				return Display.displays[id].renderingHeight;
			}
			if (id == 0) return Screen.currentResolution.height;

			return -1;
		}

		private static float GetDisplayDiagonal (int id)
		{
			if (id < _displays.Count)
			{
				//Convert screen dimensions to inches & return diagonal
				float w = _displays[id].PhysicalWidth / 25.4f;
				float h = _displays[id].PhysicalHeight / 25.4f;

				return Mathf.Sqrt(w * w + h * h);
			}

			return -1;
		}

		private static float GetDisplayDPI (int id)
		{
			if (id < _displays.Count)
			{
				float w = _displays[id].PixelWidth;
				float h = _displays[id].PixelHeight;
				float diag = Mathf.Sqrt(w * w + h * h);

				return diag / GetDisplayDiagonal(id);
			}

			return -1;
		}

		private static float GetScalingFactor (int id)
		{
			if (id < _displays.Count)
			{
				return _displays[id].ScreenWidth / _displays[id].VirtualWidth;
			}

			return -1;
		}

		#endregion
	}

	public struct DisplayData
	{
		public DisplayData (int[] info)
		{
			ScreenWidth = info[2];
			ScreenHeight = info[3];
			PixelWidth = info[4];
			PixelHeight = info[5];
			VirtualWidth = info[0];
			VirtualHeight = info[1];
			PhysicalWidth = info[6] * 0.001f;
			PhysicalHeight = info[7] * 0.001f;
			Bounds = new Rect(info[8], info[9], info[10], info[11]);
		}

		public int ScreenWidth { get; }
		public int ScreenHeight { get; }
		public int PixelWidth { get; }
		public int PixelHeight { get; }
		public int VirtualWidth { get; }
		public int VirtualHeight { get; }
		public float PhysicalWidth { get; }
		public float PhysicalHeight { get; }
		public Rect Bounds { get; }
	}
}